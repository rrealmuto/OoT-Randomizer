use {
    std::{
        borrow::Cow,
        collections::{
            VecDeque,
            hash_map::DefaultHasher,
        },
        hash::{
            Hash as _,
            Hasher as _,
        },
        iter,
        mem,
    },
    pyo3::{
        create_exception,
        exceptions::*,
        prelude::*,
        pyclass::CompareOp,
        types::{
            PyBool,
            PyType,
        },
    },
    crate::entrance_shuffle::EntranceKind,
};

macro_rules! hint_areas {
    ($($variant:ident, $vague:expr, $clear:expr, $display:expr, $short:expr, $color:expr, $dungeon:expr);* $(;)?) => {
        #[pyclass]
        #[allow(non_camel_case_types)] //TODO
        #[derive(Clone, Copy, PartialEq, Eq, Hash)]
        enum HintArea {
            $($variant),*
        }

        #[pymethods]
        impl HintArea {
            #[staticmethod]
            fn all_dungeons() -> Vec<Self> {
                [$(Option::<&str>::from($dungeon).map(|_| Self::$variant)),*].into_iter().flatten().collect()
            }

            // Peforms a breadth first search to find the closest hint area from a given spot (region, location, or entrance).
            // May fail to find a hint if the given spot is only accessible from the root and not from any other region with a hint area
            #[staticmethod]
            #[pyo3(signature = (spot, use_alt_hint = false, gc_woods_warp_is_forest = false))]
            fn at(py: Python<'_>, spot: &PyAny, use_alt_hint: bool, gc_woods_warp_is_forest: bool) -> PyResult<Self> {
                let region_class = py.import("Region")?.getattr("Region")?.downcast()?;
                let original_parent = if spot.is_instance(region_class)? {
                    spot
                } else {
                    spot.getattr("parent_region")?
                };
                let mut already_checked = Vec::default();
                let mut spot_queue = iter::once(spot).collect::<VecDeque<_>>();
                let mut fallback_spot_queue = VecDeque::default();
                while let Some(current_spot) = spot_queue.pop_front() {
                    already_checked.push(current_spot);
                    let parent_region = if current_spot.is_instance(region_class)? {
                        current_spot
                    } else {
                        current_spot.getattr("parent_region")?
                    };
                    if original_parent.getattr("name")?.eq("Root")? || parent_region.getattr("name")?.ne("Root")? {
                        if use_alt_hint {
                            if let Ok(alt_hint) = parent_region.getattr("alt_hint")?.extract() {
                                return Ok(alt_hint)
                            }
                        }
                        if gc_woods_warp_is_forest && parent_region.getattr("name")?.eq("GC Woods Warp")? {
                            return Ok(Self::LOST_WOODS)
                        }
                        if let Ok(parent_hint) = parent_region.getattr("hint")?.extract() {
                            return Ok(parent_hint)
                        }
                    }
                    'add_entrances: for entrance in parent_region.getattr("entrances")?.iter()? {
                        // add entrance to spot queue unless already checked
                        let entrance = entrance?;
                        for checked_ent in &already_checked {
                            if entrance.eq(checked_ent)? {
                                continue 'add_entrances
                            }
                        }
                        // prioritize two-way entrances
                        if let Some(EntranceKind::OverworldOneWay | EntranceKind::OwlDrop | EntranceKind::ChildSpawn | EntranceKind::AdultSpawn | EntranceKind::WarpSong | EntranceKind::BlueWarp) = entrance.getattr("type")?.extract()? {
                            fallback_spot_queue.push_back(entrance);
                        } else {
                            spot_queue.push_back(entrance);
                        }
                    }
                    if spot_queue.is_empty() {
                        mem::swap(&mut spot_queue, &mut fallback_spot_queue);
                    }
                }
                Err(HintAreaNotFound::new_err(format!("No hint area could be found for {spot} [World {}]", spot.getattr("world")?.getattr("id")?)))
            }

            #[classmethod]
            fn for_dungeon(cls: &Bound<'_, PyType>, mut dungeon_name: &str) -> Option<Self> {
                if let Some((_, part)) = dungeon_name.split_once('(') {
                    if let Some((part, _)) = part.split_once(')') {
                        // A dungeon item name was passed in - get the name of the dungeon from it.
                        dungeon_name = part;
                    }
                }

                if dungeon_name == "Thieves Hideout" {
                    // Special case for Thieves' Hideout since it's not considered a dungeon
                    return Some(Self::THIEVES_HIDEOUT)
                }

                if dungeon_name == "Treasure Chest Game" {
                    // Special case for Treasure Chest Game keys: treat them as part of the market hint area regardless of where the treasure box shop actually is.
                    return Some(Self::MARKET)
                }

                $(
                    if let Some(iter_dungeon_name) = Option::<&str>::from($dungeon) {
                        if dungeon_name.contains(iter_dungeon_name) {
                            return Some(Self::$variant)
                        }
                    }
                )*
                None
            }

            #[staticmethod]
            fn from_str(s: &str) -> PyResult<Self> {
                match s {
                    $(stringify!($variant) => Ok(Self::$variant),)*
                    _ => Err(PyKeyError::new_err(format!("No such hint area: {s}"))),
                }
            }

            // override since default impl raises on foreign rhs
            fn __richcmp__(&self, py: Python<'_>, rhs: &Self, op: CompareOp) -> Py<PyAny> {
                match op {
                    CompareOp::Eq => PyBool::new(py, self == rhs).into(),
                    CompareOp::Ne => PyBool::new(py, self != rhs).into(),
                    _ => py.NotImplemented(),
                }
            }

            fn __hash__(&self) -> isize {
                let mut hasher = DefaultHasher::new();
                self.hash(&mut hasher);
                hasher.finish() as _
            }

            fn __str__(&self) -> &'static str {
                match self {
                    $(Self::$variant => $display),*
                }
            }

            /// used for dungeon reward locations in the pause menu
            #[getter]
            fn short_name(&self) -> Option<&'static str> {
                match self {
                    $(Self::$variant => $short.into()),*
                }
            }

            /// Hint areas are further grouped into colored sections of the map by association with the medallions.
            ///
            /// These colors are used to generate the text boxes for shuffled warp songs.
            #[getter]
            fn color(&self) -> &'static str {
                match self {
                    $(Self::$variant => $color),*
                }
            }

            #[getter]
            fn dungeon_name(&self) -> Option<&'static str> {
                match self {
                    $(Self::$variant => $dungeon.into()),*
                }
            }

            #[getter]
            fn is_dungeon(&self) -> bool {
                self.dungeon_name().is_some()
            }

            fn dungeon<'p>(&self, world: &'p PyAny) -> PyResult<Option<&'p PyAny>> {
                for dungeon in world.getattr("dungeons")?.iter()? {
                    let dungeon = dungeon?;
                    if dungeon.getattr("name")?.eq(self.dungeon_name())? {
                        return Ok(Some(dungeon))
                    }
                }
                Ok(None)
            }

            fn is_dungeon_item(&self, item: &PyAny) -> PyResult<bool> {
                for dungeon in item.getattr("world")?.getattr("dungeons")?.iter()? {
                    let dungeon = dungeon?;
                    if dungeon.getattr("name")?.eq(self.dungeon_name())? {
                        return dungeon.call_method1("is_dungeon_item", (item,))?.extract()
                    }
                }
                Ok(false)
            }

            fn preposition(&self, clearer_hints: bool) -> Option<&'static str> {
                match (self, clearer_hints) {
                    $(
                        (Self::$variant, false) => $vague.into(),
                        (Self::$variant, true) => $clear.into(),
                    )*
                }
            }

            /// Formats the hint text for this area with proper grammar.
            ///
            /// Dungeons are hinted differently depending on the `clearer_hints` setting.
            #[pyo3(signature = (clearer_hints, preposition = false, use_2nd_person = false, world = None))]
            fn text<'p>(&self, py: Python<'p>, clearer_hints: bool, preposition: bool, use_2nd_person: bool, world: Option<u8>) -> PyResult<Cow<'p, str>> {
                let mut text = Cow::Borrowed(if self.is_dungeon() {
                    py.import("HintList")?.call_method1("get_hint", (self.dungeon_name(), clearer_hints))?.getattr("text")?.extract()?
                } else {
                    self.__str__()
                });
                if let Some(world) = world {
                    text = Cow::Owned(text.replace('#', ""));
                    text = Cow::Owned(if let Some((prefix, suffix)) = text.split_once(' ') {
                        match prefix {
                            "a" | "an" | "the" => format!("#world {world}'s {suffix}#"),
                            "outside" | "inside" => format!("#{prefix} world {world}'s {suffix}#"),
                            "Link's" => format!("#player {world}'s {suffix}#"),
                            _ => format!("#world {world}'s {text}"),
                        }
                    } else {
                        format!("#world {world}'s {text}#")
                    });
                } else {
                    if let Some(suffix) = text.strip_prefix("Link's ") {
                        text = Cow::Owned(if use_2nd_person {
                            format!("your {suffix}")
                        } else {
                            format!("@'s {suffix}")
                        });
                    }
                }
                if !text.contains('#') {
                    text = Cow::Owned(format!("#{text}#"));
                }
                if preposition {
                    if let Some(preposition) = self.preposition(clearer_hints) {
                        text = Cow::Owned(format!("{preposition} {text}"))
                    }
                }
                Ok(text)
            }
        }
    };
}

hint_areas! {
    // internal name        prepositions        display name                  short name                color         internal dungeon name
    //                      vague     clear
    ROOT,                   "in",     "in",     "Link's pocket",              "Free",                   "White",      None;
    HYRULE_FIELD,           "in",     "in",     "Hyrule Field",               "Hyrule Field",           "Light Blue", None;
    LON_LON_RANCH,          "at",     "at",     "Lon Lon Ranch",              "Lon Lon Ranch",          "Light Blue", None;
    MARKET,                 "in",     "in",     "the Market",                 "Market",                 "Light Blue", None;
    TEMPLE_OF_TIME,         "inside", "inside", "the Temple of Time",         "Temple of Time",         "Light Blue", None;
    CASTLE_GROUNDS,         "on",     "on",     "the Castle Grounds",         None,                     "Light Blue", None; // required for warp songs
    HYRULE_CASTLE,          "at",     "at",     "Hyrule Castle",              "Hyrule Castle",          "Light Blue", None;
    OUTSIDE_GANONS_CASTLE,  None,     None,     "outside Ganon's Castle",     "Outside Ganon's Castle", "Light Blue", None;
    INSIDE_GANONS_CASTLE,   "inside", None,     "inside Ganon's Castle",      "Inside Ganon's Castle",  "Light Blue", "Ganons Castle";
    GANONDORFS_CHAMBER,     "in",     "in",     "Ganondorf's Chamber",        "Ganondorf's Chamber",    "Light Blue", None;
    KOKIRI_FOREST,          "in",     "in",     "Kokiri Forest",              "Kokiri Forest",          "Green",      None;
    DEKU_TREE,              "inside", "inside", "the Deku Tree",              "Deku Tree",              "Green",      "Deku Tree";
    LOST_WOODS,             "in",     "in",     "the Lost Woods",             "Lost Woods",             "Green",      None;
    SACRED_FOREST_MEADOW,   "at",     "at",     "the Sacred Forest Meadow",   "Sacred Forest Meadow",   "Green",      None;
    FOREST_TEMPLE,          "in",     "in",     "the Forest Temple",          "Forest Temple",          "Green",      "Forest Temple";
    DEATH_MOUNTAIN_TRAIL,   "on",     "on",     "the Death Mountain Trail",   "Death Mountain Trail",   "Red",        None;
    DODONGOS_CAVERN,        "within", "in",     "Dodongo's Cavern",           "Dodongo's Cavern",       "Red",        "Dodongos Cavern";
    GORON_CITY,             "in",     "in",     "Goron City",                 "Goron City",             "Red",        None;
    DEATH_MOUNTAIN_CRATER,  "in",     "in",     "the Death Mountain Crater",  "Death Mountain Crater",  "Red",        None;
    FIRE_TEMPLE,            "on",     "in",     "the Fire Temple",            "Fire Temple",            "Red",        "Fire Temple";
    ZORA_RIVER,             "at",     "at",     "Zora's River",               "Zora's River",           "Blue",       None;
    ZORAS_DOMAIN,           "at",     "at",     "Zora's Domain",              "Zora's Domain",          "Blue",       None;
    ZORAS_FOUNTAIN,         "at",     "at",     "Zora's Fountain",            "Zora's Fountain",        "Blue",       None;
    JABU_JABUS_BELLY,       "in",     "inside", "Jabu Jabu's Belly",          "Jabu Jabu's Belly",      "Blue",       "Jabu Jabus Belly";
    ICE_CAVERN,             "inside", "in"    , "the Ice Cavern",             "Ice Cavern",             "Blue",       "Ice Cavern";
    LAKE_HYLIA,             "at",     "at",     "Lake Hylia",                 "Lake Hylia",             "Blue",       None;
    WATER_TEMPLE,           "under",  "in",     "the Water Temple",           "Water Temple",           "Blue",       "Water Temple";
    KAKARIKO_VILLAGE,       "in",     "in",     "Kakariko Village",           "Kakariko Village",       "Pink",       None;
    BOTTOM_OF_THE_WELL,     "within", "at",     "the Bottom of the Well",     "Bottom of the Well",     "Pink",       "Bottom of the Well";
    GRAVEYARD,              "in",     "in",     "the Graveyard",              "Graveyard",              "Pink",       None;
    SHADOW_TEMPLE,          "within", "in",     "the Shadow Temple",          "Shadow Temple",          "Pink",       "Shadow Temple";
    GERUDO_VALLEY,          "at",     "at",     "Gerudo Valley",              "Gerudo Valley",          "Yellow",     None;
    GERUDO_FORTRESS,        "at",     "at",     "Gerudo's Fortress",          "Gerudo's Fortress",      "Yellow",     None;
    THIEVES_HIDEOUT,        "in",     "in",     "the Thieves' Hideout",       "Thieves' Hideout",       "Yellow",     None;
    GERUDO_TRAINING_GROUND, "within", "on",     "the Gerudo Training Ground", "Gerudo Training Ground", "Yellow",     "Gerudo Training Ground";
    HAUNTED_WASTELAND,      "in",     "in",     "the Haunted Wasteland",      "Haunted Wasteland",      "Yellow",     None;
    DESERT_COLOSSUS,        "at",     "at",     "the Desert Colossus",        "Desert Colossus",        "Yellow",     None;
    SPIRIT_TEMPLE,          "inside", "in",     "the Spirit Temple",          "Spirit Temple",          "Yellow",     "Spirit Temple";
}

create_exception!(hints, HintAreaNotFound, PyRuntimeError);

pub(crate) fn module(py: Python<'_>) -> PyResult<&PyModule> {
    let m = PyModule::new(py, "hints")?;
    m.add_class::<HintArea>()?;
    m.add("HintAreaNotFound", py.get_type::<HintAreaNotFound>())?;
    Ok(m)
}
