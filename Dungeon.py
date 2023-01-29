import os

from Hints import HintArea
from Utils import data_path


class Dungeon:
    def __init__(self, world, name, hint):
        self.world = world
        self.name = name
        self.hint = hint
        self.regions = []
        self.boss_key = []
        self.small_keys = []
        self.dungeon_items = []
        self.silver_rupees = []
        self.reward = []

        for region in world.regions:
            if region.dungeon == self.name:
                region.dungeon = self
                self.regions.append(region)


    def copy(self, new_world):
        new_dungeon = Dungeon(new_world, self.name, self.hint)

        new_dungeon.boss_key = [item.copy(new_world) for item in self.boss_key]
        new_dungeon.small_keys = [item.copy(new_world) for item in self.small_keys]
        new_dungeon.dungeon_items = [item.copy(new_world) for item in self.dungeon_items]
        new_dungeon.silver_rupees = [item.copy(new_world) for item in self.silver_rupees]
        new_dungeon.reward = [item.copy(new_world) for item in self.reward]

        return new_dungeon


    @staticmethod
    def from_vanilla_reward(item):
        dungeons = [dungeon for dungeon in item.world.dungeons if dungeon.vanilla_reward == item.name]
        if dungeons:
            return dungeons[0]


    @property
    def keys(self):
        return self.small_keys + self.boss_key


    @property
    def all_items(self):
        return self.dungeon_items + self.keys + self.silver_rupees + self.reward


    def item_name(self, text):
        return f"{text} ({self.name})"


    @property
    def vanilla_boss_name(self):
        return {
            'Deku Tree': 'Queen Gohma',
            'Dodongos Cavern': 'King Dodongo',
            'Jabu Jabus Belly': 'Barinade',
            'Forest Temple': 'Phantom Ganon',
            'Fire Temple': 'Volvagia',
            'Water Temple': 'Morpha',
            'Shadow Temple': 'Bongo Bongo',
            'Spirit Temple': 'Twinrova',
        }.get(self.name)


    @property
    def vanilla_reward(self):
        if self.vanilla_boss_name is not None:
            return self.world.get_location(self.vanilla_boss_name).vanilla_item


    def is_dungeon_item(self, item):
        return item.name in [dungeon_item.name for dungeon_item in self.all_items]


    def __str__(self):
        return self.name


def create_dungeons(world):
    savewarps_to_connect = []
    for hint_area in HintArea:
        if hint_area.is_dungeon:
            name = hint_area.dungeon_name

            if world.settings.logic_rules == 'glitched':
                if not world.dungeon_mq[name]:
                    dungeon_json = os.path.join(data_path('Glitched World'), name + '.json')
                else:
                    dungeon_json = os.path.join(data_path('Glitched World'), name + ' MQ.json')
            else:
                if not world.dungeon_mq[name]:
                    dungeon_json = os.path.join(data_path('World'), name + '.json')
                else:
                    dungeon_json = os.path.join(data_path('World'), name + ' MQ.json')

            savewarps_to_connect += world.load_regions_from_json(dungeon_json)
            world.dungeons.append(Dungeon(world, name, hint_area))
    return savewarps_to_connect
