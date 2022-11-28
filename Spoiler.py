from collections import OrderedDict
import random

from LocationList import location_sort_order

HASH_ICONS = [
    'Deku Stick',
    'Deku Nut',
    'Bow',
    'Slingshot',
    'Fairy Ocarina',
    'Bombchu',
    'Longshot',
    'Boomerang',
    'Lens of Truth',
    'Beans',
    'Megaton Hammer',
    'Bottled Fish',
    'Bottled Milk',
    'Mask of Truth',
    'SOLD OUT',
    'Cucco',
    'Mushroom',
    'Saw',
    'Frog',
    'Master Sword',
    'Mirror Shield',
    'Kokiri Tunic',
    'Hover Boots',
    'Silver Gauntlets',
    'Gold Scale',
    'Stone of Agony',
    'Skull Token',
    'Heart Container',
    'Boss Key',
    'Compass',
    'Map',
    'Big Magic',
]

class Spoiler(object):

    def __init__(self, worlds):
        self.worlds = worlds
        self.settings = worlds[0].settings
        self.playthrough = {}
        self.entrance_playthrough = {}
        self.full_playthrough = {}
        self.max_sphere = 0
        self.locations = {}
        self.entrances = []
        self.metadata = {}
        self.required_locations = {}
        self.goal_locations = {}
        self.goal_categories = {}
        self.playthrough_locations = {}
        self.hints = {world.id: {} for world in worlds}
        self.file_hash = []


    def build_file_hash(self):
        dist_file_hash = self.settings.distribution.file_hash
        for i in range(5):
            self.file_hash.append(random.randint(0,31) if dist_file_hash[i] is None else HASH_ICONS.index(dist_file_hash[i]))


    def parse_data(self):
        for (sphere_nr, sphere) in self.playthrough.items():
            sorted_sphere = [location for location in sphere]
            sort_order = {"Song": 0, "Boss": -1}
            sorted_sphere.sort(key=lambda item: (item.world.id * 10) + sort_order.get(item.type, 1))
            self.playthrough[sphere_nr] = sorted_sphere

        self.locations = {}
        for world in self.worlds:
            spoiler_locations = sorted(
                    [location for location in world.get_locations() if not location.locked and not location.type.startswith('Hint')],
                    key=lambda x: location_sort_order.get(x.name, 100000))
            self.locations[world.id] = OrderedDict([(str(location), location.item) for location in spoiler_locations])

        entrance_sort_order = {
            "ChildSpawn": 0,
            "AdultSpawn": 0,
            "WarpSong": 1,
            "OwlDrop": 2,
            "BlueWarp": 3,
            "OverworldOneWay": 4,
            "Overworld": 5,
            "DungeonSpecial": 6,
            "Dungeon": 6,
            "ChildBoss": 7,
            "AdultBoss": 7,
            "Hideout": 8,
            "SpecialInterior": 8,
            "Interior": 8,
            "Grotto": 9,
            "Grave": 9,
        }
        for (sphere_nr, sphere) in self.entrance_playthrough.items():
            sorted_sphere = [entrance for entrance in sphere]
            sorted_sphere.sort(key=lambda entrance: entrance_sort_order.get(entrance.type, -1))
            sorted_sphere.sort(key=lambda entrance: entrance.name)
            sorted_sphere.sort(key=lambda entrance: entrance.world.id)
            self.entrance_playthrough[sphere_nr] = sorted_sphere

        self.entrances = {}
        for world in self.worlds:
            spoiler_entrances = [entrance for entrance in world.get_shuffled_entrances() if entrance.primary or entrance.type == 'Overworld' or entrance.decoupled]
            spoiler_entrances.sort(key=lambda entrance: entrance.name)
            spoiler_entrances.sort(key=lambda entrance: entrance_sort_order.get(entrance.type, -1))
            self.entrances[world.id] = spoiler_entrances
