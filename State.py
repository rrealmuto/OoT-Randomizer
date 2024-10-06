from __future__ import annotations
from collections.abc import Iterable
from typing import TYPE_CHECKING, Optional, Any

from Item import Item, ItemInfo
from RulesCommon import AccessRule, escape_name
from Boulders import BOULDER_TYPE
from Location import Location, LocationFactory

if TYPE_CHECKING:
    from Goals import GoalCategory, Goal
    from Location import Location
    from Search import Search
    from World import World

from Scene import get_scene_group, scene_groups, scene_list
from EnemizerList import enemy_actor_types, named_rooms

Triforce_Piece: int = ItemInfo.solver_ids['Triforce_Piece']
Triforce: int = ItemInfo.solver_ids['Triforce']
Rutos_Letter: int = ItemInfo.solver_ids['Rutos_Letter']
Piece_of_Heart: int = ItemInfo.solver_ids['Piece_of_Heart']

Ocarina_A_Button: int = ItemInfo.solver_ids['Ocarina_A_Button']
Ocarina_C_left_Button: int = ItemInfo.solver_ids['Ocarina_C_left_Button']
Ocarina_C_up_Button: int = ItemInfo.solver_ids['Ocarina_C_up_Button']
Ocarina_C_down_Button: int = ItemInfo.solver_ids['Ocarina_C_down_Button']
Ocarina_C_right_Button: int = ItemInfo.solver_ids['Ocarina_C_right_Button']

Megaton_Hammer: int = ItemInfo.solver_ids['Megaton_Hammer']
Progressive_Strength_Upgrade: int = ItemInfo.solver_ids['Progressive_Strength_Upgrade']
Bomb_Bag: int = ItemInfo.solver_ids['Bomb_Bag']

class State:
    def __init__(self, parent: World) -> None:
        self.solv_items: list[int] = [0] * len(ItemInfo.solver_ids)
        self.world: World = parent
        self.search: Optional[Search] = None

        self.can_blast_or_smash: AccessRule = self.world.parser.parse_rule("can_blast_or_smash")
        self.Blue_Fire: AccessRule = self.world.parser.parse_rule("Blue_Fire")

    def copy(self, new_world: Optional[World] = None) -> State:
        new_world = new_world if new_world else self.world
        new_state = State(new_world)
        for i, val in enumerate(self.solv_items):
            new_state.solv_items[i] = val
        return new_state

    def item_name(self, location: str | Location) -> Optional[str]:
        location = self.world.get_location(location)
        if location.item is None:
            return None
        return location.item.name

    def won(self) -> bool:
        return self.won_triforce_hunt() if self.world.settings.triforce_hunt else self.won_normal()

    def won_triforce_hunt(self) -> bool:
        return self.has(Triforce_Piece, self.world.settings.triforce_goal_per_world)

    def won_normal(self) -> bool:
        return self.has(Triforce)

    def has(self, item: int, count: int = 1) -> bool:
        return self.solv_items[item] >= count

    def has_any_of(self, items: Iterable[int]) -> bool:
        for i in items:
            if self.solv_items[i]:
                return True
        return False

    def has_all_of(self, items: Iterable[int]) -> bool:
        for i in items:
            if not self.solv_items[i]:
                return False
        return True

    def count_of(self, items: Iterable[int]) -> int:
        s = 0
        for i in items:
            s += self.solv_items[i]
        return s

    def item_count(self, item: int) -> int:
        return self.solv_items[item]

    def item_name_count(self, name: str) -> int:
        return self.solv_items[ItemInfo.solver_ids[escape_name(name)]]

    def has_bottle(self, **kwargs) -> bool:
        # Extra Ruto's Letter are automatically emptied
        return self.has_any_of(ItemInfo.bottle_ids) or self.has(Rutos_Letter, 2)

    def has_hearts(self, count: int, **kwargs) -> bool:
        # Warning: This is limited by World.max_progressions so it currently only works if hearts are required for LACS, bridge, or Ganon bk
        return self.heart_count() >= count

    def heart_count(self, **kwargs) -> int:
        # Warning: This is limited by World.max_progressions so it currently only works if hearts are required for LACS, bridge, or Ganon bk
        return (
            self.item_count(Piece_of_Heart) // 4 # aliases ensure Heart Container and Piece of Heart (Treasure Chest Game) are included in this
            + 3 # starting hearts
        )

    def has_medallions(self, count: int, **kwargs) -> bool:
        return self.count_of(ItemInfo.medallion_ids) >= count

    def has_stones(self, count: int, **kwargs) -> bool:
        return self.count_of(ItemInfo.stone_ids) >= count


    def has_dungeon_rewards(self, count: int, **kwargs) -> bool:
        return (self.count_of(ItemInfo.medallion_ids) + self.count_of(ItemInfo.stone_ids)) >= count

    def has_ocarina_buttons(self, count: int, **kwargs) -> bool:
        return (self.count_of(ItemInfo.ocarina_buttons_ids)) >= count

    # TODO: Store the item's solver id in the goal
    def has_item_goal(self, item_goal: dict[str, Any], **kwargs) -> bool:
        return self.solv_items[ItemInfo.solver_ids[escape_name(item_goal['name'])]] >= item_goal['minimum']

    def has_full_item_goal(self, category: GoalCategory, goal: Goal, item_goal: dict[str, Any], **kwargs) -> bool:
        local_goal = self.world.goal_categories[category.name].get_goal(goal.name)
        per_world_max_quantity = local_goal.get_item(item_goal['name'])['quantity']
        return self.solv_items[ItemInfo.solver_ids[escape_name(item_goal['name'])]] >= per_world_max_quantity

    def has_all_item_goals(self, **kwargs) -> bool:
        for category in self.world.goal_categories.values():
            for goal in category.goals:
                if not all(map(lambda i: self.has_full_item_goal(category, goal, i), goal.items)):
                    return False
        return True

    def had_night_start(self, **kwargs) -> bool:
        stod = self.world.settings.starting_tod
        # These are all not between 6:30 and 18:00
        if (stod == 'sunset' or         # 18
            stod == 'evening' or        # 21
            stod == 'midnight' or       # 00
            stod == 'witching-hour'):   # 03
            return True
        else:
            return False

    # Used for fall damage and other situations where damage is unavoidable
    def can_live_dmg(self, hearts: int, **kwargs) -> bool:
        mult = self.world.settings.damage_multiplier
        if hearts*4 >= 3:
            return mult != 'ohko' and mult != 'quadruple'
        elif hearts*4 < 3:
            return mult != 'ohko'
        else:
            return True

    # Use the guarantee_hint rule defined in json.
    def guarantee_hint(self) -> bool:
        return self.world.parser.parse_rule('guarantee_hint')(self)

    # Be careful using this function. It will not collect any
    # items that may be locked behind the item, only the item itself.
    def collect(self, item: Item) -> None:
        if item.solver_id is None:
            raise Exception(f"Item '{item.name}' lacks a `solver_id` and can not be used in `State.collect()`.")
        if 'Small Key Ring' in item.name:
            dungeon_name = item.name[:-1].split(' (', 1)[1]
            if self.world.keyring_give_bk(dungeon_name):
                bk = f'Boss Key ({dungeon_name})'
                self.solv_items[ItemInfo.solver_ids[escape_name(bk)]] = 1
        if item.alias and item.alias_id is not None:
            self.solv_items[item.alias_id] += item.alias[1]
        self.solv_items[item.solver_id] += 1

    # Be careful using this function. It will not uncollect any
    # items that may be locked behind the item, only the item itself.
    def remove(self, item: Item) -> None:
        if item.solver_id is None:
            raise Exception(f"Item '{item.name}' lacks a `solver_id` and can not be used in `State.remove()`.")
        if 'Small Key Ring' in item.name:
            dungeon_name = item.name[:-1].split(' (', 1)[1]
            if self.world.keyring_give_bk(dungeon_name):
                bk = f'Boss Key ({dungeon_name})'
                self.solv_items[ItemInfo.solver_ids[escape_name(bk)]] = 0
        if item.alias and item.alias_id is not None and self.solv_items[item.alias_id] > 0:
            self.solv_items[item.alias_id] -= item.alias[1]
            if self.solv_items[item.alias_id] < 0:
                self.solv_items[item.alias_id] = 0
        if self.solv_items[item.solver_id] > 0:
            self.solv_items[item.solver_id] -= 1

    def region_has_shortcuts(self, region_name: str, **kwargs) -> bool:
        return self.world.region_has_shortcuts(region_name)

    def has_soul(self, enemy: str, **kwargs) -> bool:
        # Get the spot (this can be a location, an entrance (region transition), or an Event)
        spot = kwargs['spot']
        
        if self.world.settings.shuffle_enemy_spawns == 'regional': # Regional soul shuffle so determine the region soul from the spot's parent region's scene
            scene = None
            # Other types of locations we need to be a bit creative
            # All regions should be marked with either a scene or a dungeon
            # We can resolve the scene id from those hopefully
            if type(spot) is Location and spot.scene == 62: # Grotto locations
                scene = "Grottos"
            elif spot.parent_region.dungeon:
                scene = spot.parent_region.dungeon.name
            elif type(spot) is Location and spot.scene and spot.scene != 0xFF and spot.scene != 62: # For actual item locations we can get the scene from the location directly
                scene = scene_list[spot.scene]
            elif spot.parent_region.scene:
                scene = spot.parent_region.scene
            if scene is not None:
                # We have a scene name so loop through our scene groups
                group = get_scene_group(scene)
            soul_str = group + " Souls"
            return self.has(ItemInfo.solver_ids[escape_name(soul_str)])
        else:
            soul_str = enemy + " Soul"
        return (not self.world.shuffle_enemy_spawns or self.has(ItemInfo.solver_ids[escape_name(soul_str)]))

    # Logic helper for determining if an enemy at a partciular spot can be killed, only use for enemy drop shuffle
    def can_kill_this(self, **kwargs) -> bool:
        spot = kwargs['spot']
        if type(spot) is not Location or spot.type != 'EnemyDrop':
            raise Exception("Can't use can_kill_this for non EnemyDrop accessibility checks")
        # Get the key from LocationList
        scene = spot.scene
        # Handful of locations with multiple setups. Always just use the first one
        if type(spot.default) is list:
            default = spot.default[0]
        else:
            default = spot.default
        room,setup,index = default
        if scene == 0x3E: # Grotto scene so don't care about setup
            setup = 0
        index -= 1 # Keys from LocationList are 1-indexed so subtract 1
        # Get the enemy type for this location
        
        return self.can_kill_with_drop(scene,room,setup,index, **kwargs)
    
    def enemy_type_at(self, location_name:str, **kwargs):
        spot = LocationFactory(location_name)
        scene = spot.scene
        room,setup,index = spot.default
        index -= 1
        enemies = self.world.enemies_by_scene[scene][room][setup]
        enemy_obj, shuffled = enemies[scene,room,setup,index]
        return enemy_obj.name

    def can_kill_with_drop(self, scene, room, setup, index, **kwargs) -> bool:
        enemies = self.world.enemies_by_scene[scene][room][setup]
        enemy_obj, shuffled = enemies[scene,room,setup,index]
        # Check soul for this enemy
        has_soul = self.has_soul(enemy_obj.soul_name, **kwargs)
    
        # TODO Build an ID -> Defeatibility check mapping
        # Check defeatibility
        can_kill = self.world.parser.parse_rule(enemy_obj.drop_logic)(self, **kwargs)

        return has_soul and can_kill

    # Logic helper for determining if an enemy at a particular spot can be killed. Used when logic for one spot depends on killing a specific enemy
    def can_kill(self, scene,room,setup,index, **kwargs) -> bool:
        enemies = self.world.enemies_by_scene[scene][room][setup]
        enemy_obj, shuffled = enemies[scene,room,setup,index]
        # Check soul for this enemy
        has_soul = self.has_soul(enemy_obj.soul_name, **kwargs)
    
        # TODO Build an ID -> Defeatibility check mapping
        # Check defeatibility
        can_kill = self.world.parser.parse_rule(enemy_obj.kill_logic)(self, **kwargs)

        return has_soul and can_kill

    def can_kill_named(self, location_name:str, **kwargs):
        spot = LocationFactory(location_name)
        if type(spot) is not Location or spot.type != 'EnemyDrop':
            raise Exception("Can't use can_kill_this for non EnemyDrop accessibility checks")
        # Get the key from LocationList
        scene = spot.scene
        room,setup,index = spot.default
        if scene == 0x3E: # Grotto scene so don't care about setup
            setup = 0
        index -= 1 # Keys from LocationList are 1-indexed so subtract 1
        # Get the enemy type for this location
        
        return self.can_kill(scene,room,setup,index, **kwargs)

    # Logic helper for determining if a room/scene/setup can be cleared
    def can_clear_room_setup(self, scene,room,setup, **kwargs) -> bool:
        # Get the enemies for this scene/room/setup
        enemies = self.world.enemies_by_scene[scene][room][setup]
        # Loop through each enemy and determine defeatability
        # Need to check for the soul for each enemy, and the defeatability function
        for enemy in enemies:
            enemy_obj, shuffled = enemies[enemy]
            if not self.has_soul(enemy_obj.soul_name, **kwargs):
                return False

        return True
    
    #def can_clear_room(self, scene, room, **kwargs) -> bool:
    #    return self.can_clear_room_setup(scene,room,0, **kwargs)
    
    def can_clear_room(self, room:str, **kwargs) -> bool:
        tup = named_rooms[room]
        if len(tup) == 2:
            scene, room = tup
            setup = 0
        elif len(tup) == 3:
            scene,room,setup = tup
        return self.can_clear_room_setup(scene,room,setup,**kwargs)

    def has_all_notes_for_song(self, song: str, **kwargs) -> bool:
        # Scarecrow needs 2 different notes
        if song == 'Scarecrow Song':
            return self.has_ocarina_buttons(2)

        notes = str(self.world.song_notes[song])
        if 'A' in notes:
            if not self.has(Ocarina_A_Button):
                return False
        if '<' in notes:
            if not self.has(Ocarina_C_left_Button):
                return False
        if '^' in notes:
            if not self.has(Ocarina_C_up_Button):
                return False
        if 'v' in notes:
            if not self.has(Ocarina_C_down_Button):
                return False
        if '>' in notes:
            if not self.has(Ocarina_C_right_Button):
                return False
        return True

    def __getstate__(self) -> dict[str, Any]:
        return self.__dict__.copy()

    def __setstate__(self, state: dict[str, Any]) -> None:
        self.__dict__.update(state)

    def get_prog_items(self) -> dict[str, int]:
        return {
            **{item.name: self.solv_items[item.solver_id]
                for item in ItemInfo.items.values()
                if item.solver_id is not None},
            **{event: self.solv_items[ItemInfo.solver_ids[event]]
                for event in self.world.event_items
                if self.solv_items[ItemInfo.solver_ids[event]]},
        }

    def boulder_type(self, boulder: str, **kwargs) -> BOULDER_TYPE:
        return self.world.boulders[boulder]

    def can_pass_boulder(self, boulder: str, age: str, **kwargs) -> bool:
        type: BOULDER_TYPE = self.world.boulders[boulder]
        return self.can_pass_boulder_type(type, age, kwargs=kwargs)

    def can_pass_boulder_type(self, boulder_type:BOULDER_TYPE, age: str, **kwargs) -> bool:
        if boulder_type == BOULDER_TYPE.BRONZE:
            # Check for hammer and adult
            return age == 'adult' and self.has(Megaton_Hammer)
        elif boulder_type == BOULDER_TYPE.SILVER:
            # Check for adult+str2
            return age == 'adult' and self.has(Progressive_Strength_Upgrade, 2)
        elif boulder_type == BOULDER_TYPE.BROWN:
            # Check for adult+hammer or explosives
            return self.can_blast_or_smash(self, age=age)
        elif boulder_type == BOULDER_TYPE.RED_ICE:
            # Check for blue fire
            #return self.world.parser.parse_rule('Blue_Fire')(self, age=age)
            return self.Blue_Fire(self, age=age)
        
        # Should never get here
        return False

    # Check if the current state can pass a particular boulder, restricted to a list of types
    def can_pass_boulder_types(self, boulder: str, types: list[BOULDER_TYPE], age: str, **kwargs) -> bool:
        this_boulder_type: BOULDER_TYPE = self.world.boulders[boulder] # Get the boulder's type
        if this_boulder_type in types: # Check if that type is in the list of allowed types
            return self.can_pass_boulder_type(this_boulder_type, age, kwargs=kwargs) # Check ability to pass that type
        return False