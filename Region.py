from enum import Enum, unique
from ItemPool import triforce_items


@unique
class RegionType(Enum):

    Overworld = 1
    Interior = 2
    Dungeon = 3
    Grotto = 4


    @property
    def is_indoors(self):
        """Shorthand for checking if Interior or Dungeon"""
        return self in (RegionType.Interior, RegionType.Dungeon, RegionType.Grotto)


# Pretends to be an enum, but when the values are raw ints, it's much faster
class TimeOfDay(object):
    NONE = 0
    DAY = 1
    DAMPE = 2
    ALL = DAY | DAMPE


class Region(object):

    def __init__(self, name, type=RegionType.Overworld):
        self.name = name
        self.type = type
        self.entrances = []
        self.exits = []
        self.locations = []
        self.dungeon = None
        self.world = None
        self.hint_name = None
        self.price = None
        self.world = None
        self.time_passes = False
        self.provides_time = TimeOfDay.NONE
        self.scene = None


    def copy(self, new_world):
        new_region = Region(self.name, self.type)
        new_region.world = new_world
        new_region.price = self.price
        new_region.hint_name = self.hint_name
        new_region.time_passes = self.time_passes
        new_region.provides_time = self.provides_time
        new_region.scene = self.scene

        if self.dungeon:
            new_region.dungeon = self.dungeon.name
        new_region.locations = [location.copy(new_region) for location in self.locations]
        new_region.exits = [exit.copy(new_region) for exit in self.exits]

        return new_region


    @property
    def hint(self):
        from Hints import HintArea

        if self.hint_name is not None:
            return HintArea[self.hint_name]
        if self.dungeon:
            return self.dungeon.hint


    def can_fill(self, item, manual=False):
        if not manual and self.world.settings.empty_dungeons_mode != 'none' and item.dungeonitem:
            # An empty dungeon can only store its own dungeon items
            if self.dungeon and self.dungeon.world.empty_dungeons[self.dungeon.name].empty:
                return self.dungeon.is_dungeon_item(item) and item.world.id == self.world.id
            # Items from empty dungeons can only be in their own dungeons
            for dungeon in item.world.dungeons:
                if item.world.empty_dungeons[dungeon.name].empty and dungeon.is_dungeon_item(item):
                    return False
        
        from Hints import HintArea

        is_self_dungeon_restricted = False
        is_dungeon_restricted = False
        is_overworld_restricted = False
        if item.map or item.compass:
            is_self_dungeon_restricted = self.world.settings.shuffle_mapcompass in ['dungeon', 'vanilla']
            is_dungeon_restricted = self.world.settings.shuffle_mapcompass == 'any_dungeon'
            is_overworld_restricted = self.world.settings.shuffle_mapcompass == 'overworld'
        elif item.type == 'SmallKey':
            is_self_dungeon_restricted = self.world.settings.shuffle_smallkeys in ['dungeon', 'vanilla']
            is_dungeon_restricted = self.world.settings.shuffle_smallkeys == 'any_dungeon'
            is_overworld_restricted = self.world.settings.shuffle_smallkeys == 'overworld'
        elif item.type == 'HideoutSmallKey':
            is_dungeon_restricted = self.world.settings.shuffle_hideoutkeys == 'any_dungeon'
            is_overworld_restricted = self.world.settings.shuffle_hideoutkeys == 'overworld'
        elif item.type == 'BossKey':
            is_self_dungeon_restricted = self.world.settings.shuffle_bosskeys in ['dungeon', 'vanilla']
            is_dungeon_restricted = self.world.settings.shuffle_bosskeys == 'any_dungeon'
            is_overworld_restricted = self.world.settings.shuffle_bosskeys == 'overworld'
        elif item.type == 'GanonBossKey':
            is_self_dungeon_restricted = self.world.settings.shuffle_ganon_bosskey in ['dungeon', 'vanilla']
            is_dungeon_restricted = self.world.settings.shuffle_ganon_bosskey == 'any_dungeon'
            is_overworld_restricted = self.world.settings.shuffle_ganon_bosskey == 'overworld'

        if is_self_dungeon_restricted and not manual:
            hint_area = HintArea.at(self)
            return hint_area.is_dungeon and hint_area.is_dungeon_item(item) and item.world.id == self.world.id

        if is_dungeon_restricted and not manual:
            return HintArea.at(self).is_dungeon

        if is_overworld_restricted and not manual:
            return not HintArea.at(self).is_dungeon

        if item.name in triforce_items:
            return item.world.id == self.world.id

        return True


    def get_scene(self):
        if self.scene: 
            return self.scene
        elif self.dungeon:
            return self.dungeon.name
        else: 
            return None


    def __str__(self):
        return str(self.__unicode__())


    def __unicode__(self):
        return '%s' % self.name

