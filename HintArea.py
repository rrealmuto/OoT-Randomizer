from __future__ import annotations
from enum import Enum
from typing import Optional
from HintList import get_hint

from Item import Item
from Region import Region

Spot: TypeAlias = "Entrance | Location | Region"

class HintArea(Enum):
    # internal name          prepositions        display name                  short name                color         internal dungeon name
    #                        vague     clear
    ROOT                   = 'in',     'in',     "Link's pocket",              'Free',                   'White',      None
    HYRULE_FIELD           = 'in',     'in',     'Hyrule Field',               'Hyrule Field',           'Light Blue', None
    LON_LON_RANCH          = 'at',     'at',     'Lon Lon Ranch',              'Lon Lon Ranch',          'Light Blue', None
    MARKET                 = 'in',     'in',     'the Market',                 'Market',                 'Light Blue', None
    TEMPLE_OF_TIME         = 'inside', 'inside', 'the Temple of Time',         'Temple of Time',         'Light Blue', None
    CASTLE_GROUNDS         = 'on',     'on',     'the Castle Grounds',         None,                     'Light Blue', None # required for warp songs
    HYRULE_CASTLE          = 'at',     'at',     'Hyrule Castle',              'Hyrule Castle',          'Light Blue', None
    OUTSIDE_GANONS_CASTLE  = None,     None,     "outside Ganon's Castle",     "Outside Ganon's Castle", 'Light Blue', None
    INSIDE_GANONS_CASTLE   = 'inside', None,     "inside Ganon's Castle",      "Inside Ganon's Castle",  'Light Blue', 'Ganons Castle'
    GANONDORFS_CHAMBER     = 'in',     'in',     "Ganondorf's Chamber",        "Ganondorf's Chamber",    'Light Blue', None
    KOKIRI_FOREST          = 'in',     'in',     'Kokiri Forest',              "Kokiri Forest",          'Green',      None
    DEKU_TREE              = 'inside', 'inside', 'the Deku Tree',              "Deku Tree",              'Green',      'Deku Tree'
    LOST_WOODS             = 'in',     'in',     'the Lost Woods',             "Lost Woods",             'Green',      None
    SACRED_FOREST_MEADOW   = 'at',     'at',     'the Sacred Forest Meadow',   "Sacred Forest Meadow",   'Green',      None
    FOREST_TEMPLE          = 'in',     'in',     'the Forest Temple',          "Forest Temple",          'Green',      'Forest Temple'
    DEATH_MOUNTAIN_TRAIL   = 'on',     'on',     'the Death Mountain Trail',   "Death Mountain Trail",   'Red',        None
    DODONGOS_CAVERN        = 'within', 'in',     "Dodongo's Cavern",           "Dodongo's Cavern",       'Red',        'Dodongos Cavern'
    GORON_CITY             = 'in',     'in',     'Goron City',                 "Goron City",             'Red',        None
    DEATH_MOUNTAIN_CRATER  = 'in',     'in',     'the Death Mountain Crater',  "Death Mountain Crater",  'Red',        None
    FIRE_TEMPLE            = 'on',     'in',     'the Fire Temple',            "Fire Temple",            'Red',        'Fire Temple'
    ZORA_RIVER             = 'at',     'at',     "Zora's River",               "Zora's River",           'Blue',       None
    ZORAS_DOMAIN           = 'at',     'at',     "Zora's Domain",              "Zora's Domain",          'Blue',       None
    ZORAS_FOUNTAIN         = 'at',     'at',     "Zora's Fountain",            "Zora's Fountain",        'Blue',       None
    JABU_JABUS_BELLY       = 'in',     'inside', "Jabu Jabu's Belly",          "Jabu Jabu's Belly",      'Blue',       'Jabu Jabus Belly'
    ICE_CAVERN             = 'inside', 'in'    , 'the Ice Cavern',             "Ice Cavern",             'Blue',       'Ice Cavern'
    LAKE_HYLIA             = 'at',     'at',     'Lake Hylia',                 "Lake Hylia",             'Blue',       None
    WATER_TEMPLE           = 'under',  'in',     'the Water Temple',           "Water Temple",           'Blue',       'Water Temple'
    KAKARIKO_VILLAGE       = 'in',     'in',     'Kakariko Village',           "Kakariko Village",       'Pink',       None
    BOTTOM_OF_THE_WELL     = 'within', 'at',     'the Bottom of the Well',     "Bottom of the Well",     'Pink',       'Bottom of the Well'
    GRAVEYARD              = 'in',     'in',     'the Graveyard',              "Graveyard",              'Pink',       None
    SHADOW_TEMPLE          = 'within', 'in',     'the Shadow Temple',          "Shadow Temple",          'Pink',       'Shadow Temple'
    GERUDO_VALLEY          = 'at',     'at',     'Gerudo Valley',              "Gerudo Valley",          'Yellow',     None
    GERUDO_FORTRESS        = 'at',     'at',     "Gerudo's Fortress",          "Gerudo's Fortress",      'Yellow',     None
    THIEVES_HIDEOUT        = 'in',     'in',     "the Thieves' Hideout",       "Thieves' Hideout",       'Yellow',     None
    GERUDO_TRAINING_GROUND = 'within', 'on',     'the Gerudo Training Ground', "Gerudo Training Ground", 'Yellow',     'Gerudo Training Ground'
    HAUNTED_WASTELAND      = 'in',     'in',     'the Haunted Wasteland',      "Haunted Wasteland",      'Yellow',     None
    DESERT_COLOSSUS        = 'at',     'at',     'the Desert Colossus',        "Desert Colossus",        'Yellow',     None
    SPIRIT_TEMPLE          = 'inside', 'in',     'the Spirit Temple',          "Spirit Temple",          'Yellow',     'Spirit Temple'

    # Performs a breadth first search to find the closest hint area from a given spot (region, location, or entrance).
    # May fail to find a hint if the given spot is only accessible from the root and not from any other region with a hint area
    @staticmethod
    def at(spot: Spot, use_alt_hint: bool = False) -> HintArea:
        if isinstance(spot, Region):
            original_parent = spot
        else:
            original_parent = spot.parent_region
        already_checked = []
        spot_queue = [spot]
        fallback_spot_queue = []

        while spot_queue or fallback_spot_queue:
            if not spot_queue:
                spot_queue = fallback_spot_queue
                fallback_spot_queue = []
            current_spot = spot_queue.pop(0)
            already_checked.append(current_spot)

            if isinstance(current_spot, Region):
                parent_region = current_spot
            else:
                parent_region = current_spot.parent_region

            if parent_region.hint and (original_parent.name == 'Root' or parent_region.name != 'Root'):
                if use_alt_hint and parent_region.alt_hint:
                    return parent_region.alt_hint
                return parent_region.hint

            for entrance in parent_region.entrances:
                if entrance not in already_checked:
                    # prioritize two-way entrances
                    if entrance.type in ('OverworldOneWay', 'OwlDrop', 'Spawn', 'WarpSong'):
                        fallback_spot_queue.append(entrance)
                    else:
                        spot_queue.append(entrance)

        raise HintAreaNotFound('No hint area could be found for %s [World %d]' % (spot, spot.world.id))

    @classmethod
    def for_dungeon(cls, dungeon_name: str) -> Optional[HintArea]:
        if '(' in dungeon_name and ')' in dungeon_name:
            # A dungeon item name was passed in - get the name of the dungeon from it.
            dungeon_name = dungeon_name[dungeon_name.index('(') + 1:dungeon_name.index(')')]

        if dungeon_name == "Thieves Hideout":
            # Special case for Thieves' Hideout since it's not considered a dungeon
            return cls.THIEVES_HIDEOUT

        if dungeon_name == "Treasure Chest Game":
            # Special case for Treasure Chest Game keys: treat them as part of the market hint area regardless of where the treasure box shop actually is.
            return cls.MARKET

        for hint_area in cls:
            if hint_area.dungeon_name is not None and hint_area.dungeon_name in dungeon_name:
                return hint_area
        return None

    def preposition(self, clearer_hints: bool) -> str:
        return self.value[1 if clearer_hints else 0]

    def __str__(self) -> str:
        return self.value[2]

    # used for dungeon reward locations in the pause menu
    @property
    def short_name(self) -> str:
        return self.value[3]

    # Hint areas are further grouped into colored sections of the map by association with the medallions.
    # These colors are used to generate the text boxes for shuffled warp songs.
    @property
    def color(self) -> str:
        return self.value[4]

    @property
    def dungeon_name(self) -> Optional[str]:
        return self.value[5]

    @property
    def is_dungeon(self) -> bool:
        return self.dungeon_name is not None

    def is_dungeon_item(self, item: Item) -> bool:
        for dungeon in item.world.dungeons:
            if dungeon.name == self.dungeon_name:
                return dungeon.is_dungeon_item(item)
        return False

    # Formats the hint text for this area with proper grammar.
    # Dungeons are hinted differently depending on the clearer_hints setting.
    def text(self, clearer_hints: bool, preposition: bool = False, world: Optional[int] = None) -> str:
        if self.is_dungeon and self.dungeon_name:
            text = get_hint(self.dungeon_name, clearer_hints).text
        else:
            text = str(self)
        prefix, suffix = text.replace('#', '').split(' ', 1)
        if world is None:
            if prefix == "Link's":
                text = f"@'s {suffix}"
        else:
            replace_prefixes = ('a', 'an', 'the')
            move_prefixes = ('outside', 'inside')
            if prefix in replace_prefixes:
                text = f"world {world}'s {suffix}"
            elif prefix in move_prefixes:
                text = f"{prefix} world {world}'s {suffix}"
            elif prefix == "Link's":
                text = f"player {world}'s {suffix}"
            else:
                text = f"world {world}'s {text}"
        if '#' not in text:
            text = f'#{text}#'
        if preposition and self.preposition(clearer_hints) is not None:
            text = f'{self.preposition(clearer_hints)} {text}'
        return text
    

class HintAreaNotFound(RuntimeError):
    pass