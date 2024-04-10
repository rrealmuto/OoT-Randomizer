use {
    std::{
        cmp::Ordering::*,
        collections::{
            HashMap,
            HashSet,
        },
    },
    collect_mac::collect,
    itertools::Itertools as _,
    rand::prelude::*,
};

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
enum ItemKind {
    Bow,
    MagicMeter,
    LightArrows,
    Triforce,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
struct Item {
    kind: ItemKind,
    player: u8,
    /// Differentiate between multiple items of the same kind so they can be counted properly even with speculative collection from unfilled locations.
    idx: usize,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
enum Region {
    Root,
}

impl Region {
    fn locations(&self) -> impl IntoIterator<Item = Location> {
        match self {
            Self::Root => [
                Location::TotLightArrowCutscene,
                Location::ForestTempleBowChest,
                Location::DmtGreatFairyReward,
                Location::DmcGreatFairyReward,
                Location::KakShootingGalleryReward,
                Location::GfHba1500Points,
                Location::Ganon,
            ],
        }
    }

    fn tod_passes(&self) -> bool {
        match self {
            Self::Root => true,
        }
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
enum Location {
    TotLightArrowCutscene,
    ForestTempleBowChest,
    DmtGreatFairyReward,
    DmcGreatFairyReward,
    KakShootingGalleryReward,
    GfHba1500Points,
    Ganon,
}

impl Location {
    fn can_access(&self, world_id: u8, region_access: RegionAccess, inventories: &HashSet<Item>) -> bool {
        match self {
            Self::TotLightArrowCutscene => region_access.adult.unknown_daytime
                && inventories.iter().any(|&Item { kind, player, idx: _ }| kind == ItemKind::Bow && player == world_id)
                && inventories.iter().any(|&Item { kind, player, idx: _ }| kind == ItemKind::MagicMeter && player == world_id),
            Self::ForestTempleBowChest => region_access.adult.unknown_daytime,
            Self::DmtGreatFairyReward => true,
            Self::DmcGreatFairyReward => region_access.adult.unknown_daytime,
            Self::KakShootingGalleryReward => region_access.adult.noon
                && inventories.iter().any(|&Item { kind, player, idx: _ }| kind == ItemKind::Bow && player == world_id),
            Self::GfHba1500Points => region_access.adult.noon
                && inventories.iter().any(|&Item { kind, player, idx: _ }| kind == ItemKind::Bow && player == world_id),
            Self::Ganon => region_access.adult.unknown_daytime
                && inventories.iter().any(|&Item { kind, player, idx: _ }| kind == ItemKind::Bow && player == world_id)
                && inventories.iter().any(|&Item { kind, player, idx: _ }| kind == ItemKind::MagicMeter && player == world_id)
                && inventories.iter().any(|&Item { kind, player, idx: _ }| kind == ItemKind::LightArrows && player == world_id),
        }
    }
}

#[derive(Debug, Default, Clone, Copy)]
struct RegionAccess {
    child: RegionAgeAccess,
    maybe_child: RegionAgeAccess,
    adult: RegionAgeAccess,
    maybe_adult: RegionAgeAccess,
}

impl<'a> IntoIterator for &'a mut RegionAccess {
    type IntoIter = std::array::IntoIter<&'a mut RegionAgeAccess, 4>;
    type Item = &'a mut RegionAgeAccess;

    fn into_iter(self) -> Self::IntoIter {
        let RegionAccess { child, maybe_child, adult, maybe_adult } = self;
        [child, maybe_child, adult, maybe_adult].into_iter()
    }
}

#[derive(Debug, Default, Clone, Copy)]
struct RegionAgeAccess {
    noon: bool,
    dampe_time: bool,
    midnight: bool,
    unknown_daytime: bool,
}

#[derive(Debug)]
struct Search<'a> {
    inventories: HashSet<Item>,
    visited_locations: Vec<HashSet<Location>>,
    region_access: Vec<HashMap<Region, RegionAccess>>,
    worlds: &'a [World],
    // avoid cloning worlds by passing the modified location separately
    candidate_world: u8,
    candidate_location: Location,
    candidate_item: Item,
}

impl Search<'_> {
    /// Iteratively accesses new regions and collects items from unvisited locations until nothing new is reachable anymore.
    fn max_explore(&mut self) {
        loop {
            let mut had_progress = false;
            for world_region_access in self.region_access.iter_mut() {
                for (region, access) in world_region_access {
                    //TODO explore exits
                    // explore time-of-day transitions
                    if region.tod_passes() {
                        for age_access in access {
                            if age_access.unknown_daytime {
                                *age_access = RegionAgeAccess { noon: true, dampe_time: true, midnight: true, unknown_daytime: true };
                            }
                        }
                    }
                }
            }
            // explore locations
            for (world_id, world_region_access) in self.region_access.iter().enumerate() {
                for (region, &access) in world_region_access {
                    for location in region.locations() {
                        if !self.visited_locations[world_id].contains(&location) && location.can_access(world_id.try_into().expect("too many worlds"), access, &self.inventories) {
                            //TODO if the item should appear in spoiler log playthrough, delay collecting
                            if u8::try_from(world_id).expect("too many worlds") == self.candidate_world && location == self.candidate_location {
                                self.inventories.insert(self.candidate_item);
                            } else {
                                self.inventories.extend(self.worlds[world_id].locations[&location].iter().copied());
                            }
                            self.visited_locations[world_id].insert(location);
                            had_progress = true;
                        }
                    }
                }
            }
            if !had_progress {
                //TODO if there is any delayed-collection stuff, add it to self and continue
                break
            }
        }
    }
}

#[derive(Debug, Clone)]
struct World {
    //TODO entrances
    locations: HashMap<Location, HashSet<Item>>,
    settings: Settings,
}

#[derive(Debug, Clone)]
struct Settings {
    adult_start: bool,
}

fn fill(rng: &mut impl Rng, worlds: &mut [World], starting_items: HashSet<Item>) {
    let mut undecided_locations = worlds.iter()
        .enumerate()
        .flat_map(|(world_id, world)| world.locations.iter().filter(|(_, items)| {
            assert!(!items.is_empty());
            items.len() > 1
        }).map(move |(&location, _)| (world_id, location)))
        .collect_vec();
    'fill: while let Some((idx, _)) = undecided_locations.iter().enumerate().min_by(|(_, (world1, loc1)), (_, (world2, loc2))| worlds[*world1].locations[loc1].len().cmp(&worlds[*world2].locations[loc2].len()).then_with(|| if rng.gen() { Less } else { Greater })) {
        let (world_id, location) = undecided_locations.remove(idx);
        let mut candidates = worlds[world_id].locations[&location].iter().copied().collect_vec();
        candidates.shuffle(rng);
        for &candidate in &candidates {
            //TODO age access searches (currently assumes that both are reachable)
            let mut search = Search {
                inventories: starting_items.clone(),
                visited_locations: vec![HashSet::default(); worlds.len()],
                region_access: vec![collect![Region::Root => RegionAccess {
                    child: RegionAgeAccess { unknown_daytime: true, ..RegionAgeAccess::default() },
                    adult: RegionAgeAccess { unknown_daytime: true, ..RegionAgeAccess::default() },
                    ..RegionAccess::default()
                }]; worlds.len()],
                worlds,
                candidate_world: world_id.try_into().expect("too many worlds"),
                candidate_location: location,
                candidate_item: candidate,
            };
            search.max_explore();
            //TODO support for different wincons (ALR, AGR, TH, Bingo, etc.)
            //TODO check for ToT access, either as the non-starting age with only starting items, or as both ages without collecting locations with age requirements
            if (0..worlds.len()).all(|world_id| search.inventories.contains(&Item { kind: ItemKind::Triforce, player: world_id.try_into().expect("too many worlds"), idx: 0 })) {
                for (&iter_location, candidates) in &mut worlds[world_id].locations {
                    if iter_location == location {
                        *candidates = collect![candidate];
                    } else {
                        candidates.remove(&candidate);
                    }
                }
                continue 'fill
            }
        }
        panic!("could not fill location {location:?}, candidates: {candidates:?}")
    }
}

fn main() {
    let mut worlds = vec![World {
        locations: collect![ //TODO build from item pool
            Location::TotLightArrowCutscene => collect![Item { kind: ItemKind::Bow, player: 0, idx: 0 }, Item { kind: ItemKind::Bow, player: 0, idx: 1 }, Item { kind: ItemKind::Bow, player: 0, idx: 2 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 0 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 1 }, Item { kind: ItemKind::LightArrows, player: 0, idx: 0 }],
            Location::ForestTempleBowChest => collect![Item { kind: ItemKind::Bow, player: 0, idx: 0 }, Item { kind: ItemKind::Bow, player: 0, idx: 1 }, Item { kind: ItemKind::Bow, player: 0, idx: 2 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 0 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 1 }, Item { kind: ItemKind::LightArrows, player: 0, idx: 0 }],
            Location::DmtGreatFairyReward => collect![Item { kind: ItemKind::Bow, player: 0, idx: 0 }, Item { kind: ItemKind::Bow, player: 0, idx: 1 }, Item { kind: ItemKind::Bow, player: 0, idx: 2 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 0 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 1 }, Item { kind: ItemKind::LightArrows, player: 0, idx: 0 }],
            Location::DmcGreatFairyReward => collect![Item { kind: ItemKind::Bow, player: 0, idx: 0 }, Item { kind: ItemKind::Bow, player: 0, idx: 1 }, Item { kind: ItemKind::Bow, player: 0, idx: 2 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 0 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 1 }, Item { kind: ItemKind::LightArrows, player: 0, idx: 0 }],
            Location::KakShootingGalleryReward => collect![Item { kind: ItemKind::Bow, player: 0, idx: 0 }, Item { kind: ItemKind::Bow, player: 0, idx: 1 }, Item { kind: ItemKind::Bow, player: 0, idx: 2 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 0 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 1 }, Item { kind: ItemKind::LightArrows, player: 0, idx: 0 }],
            Location::GfHba1500Points => collect![Item { kind: ItemKind::Bow, player: 0, idx: 0 }, Item { kind: ItemKind::Bow, player: 0, idx: 1 }, Item { kind: ItemKind::Bow, player: 0, idx: 2 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 0 }, Item { kind: ItemKind::MagicMeter, player: 0, idx: 1 }, Item { kind: ItemKind::LightArrows, player: 0, idx: 0 }],
            Location::Ganon => collect![Item { kind: ItemKind::Triforce, player: 0, idx: 0 }],
        ],
        settings: Settings { adult_start: false },
    }];
    fill(&mut thread_rng(), &mut worlds, HashSet::default());
    println!("{worlds:#?}");
}
