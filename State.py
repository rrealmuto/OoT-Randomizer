from __future__ import annotations
from collections.abc import Iterable
from typing import TYPE_CHECKING, Optional, Any

from Item import Item, ItemInfo
from RulesCommon import AccessRule, escape_name
from Boulders import BOULDER_TYPE

if TYPE_CHECKING:
    from Goals import GoalCategory, Goal
    from Location import Location
    from Search import Search
    from World import World

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
        if 'Small Key Ring' in item.name and self.world.settings.keyring_give_bk:
            dungeon_name = item.name[:-1].split(' (', 1)[1]
            if dungeon_name in ('Forest Temple', 'Fire Temple', 'Water Temple', 'Shadow Temple', 'Spirit Temple'):
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
        if 'Small Key Ring' in item.name and self.world.settings.keyring_give_bk:
            dungeon_name = item.name[:-1].split(' (', 1)[1]
            if dungeon_name in ('Forest Temple', 'Fire Temple', 'Water Temple', 'Shadow Temple', 'Spirit Temple'):
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
        soul_str = enemy + " Soul"
        return (not self.world.shuffle_enemy_spawns or self.has(ItemInfo.solver_ids[escape_name(soul_str)]))

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
        if type == BOULDER_TYPE.BRONZE:
            # Check for hammer and adult
            return age == 'adult' and self.has(Megaton_Hammer)
        elif type == BOULDER_TYPE.SILVER:
            # Check for adult+str2
            return age == 'adult' and self.has(Progressive_Strength_Upgrade, 2)
        elif type == BOULDER_TYPE.BROWN:
            # Check for adult+hammer or explosives
            return self.can_blast_or_smash(self)
        elif type == BOULDER_TYPE.RED_ICE:
            # Check for blue fire
            return self.Blue_Fire(self)
