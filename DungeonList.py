import os

from Dungeon import Dungeon
from Utils import data_path


dungeon_table = [
    {
        'name': 'Deku Tree',
        'hint': 'the Deku Tree',
        'font_color': 'Green',
    },
    {
        'name': 'Dodongos Cavern',
        'hint': "Dodongo's Cavern",
        'font_color': 'Red',
    },
    {
        'name': 'Jabu Jabus Belly',
        'hint': "Jabu Jabu's Belly",
        'font_color': 'Blue',
    },
    {
        'name': 'Forest Temple',
        'hint': 'the Forest Temple',
        'font_color': 'Green',
    },
    {
        'name': 'Bottom of the Well',
        'hint': 'the Bottom of the Well',
        'font_color': 'Pink',
    },
    {
        'name': 'Fire Temple',
        'hint': 'the Fire Temple',
        'font_color': 'Red',
    },
    {
        'name': 'Ice Cavern',
        'hint': 'the Ice Cavern',
        'font_color': 'Blue',
    },
    {
        'name': 'Water Temple',
        'hint': 'the Water Temple',
        'font_color': 'Blue',
    },
    {
        'name': 'Shadow Temple',
        'hint': 'the Shadow Temple',
        'font_color': 'Pink',
    },
    {
        'name': 'Gerudo Training Ground',
        'hint': 'the Gerudo Training Ground',
        'font_color': 'Yellow',
    },
    {
        'name': 'Spirit Temple',
        'hint': 'the Spirit Temple',
        'font_color': 'Yellow',
    },
    {
        'name': 'Ganons Castle',
        'hint': "inside Ganon's Castle",
    },
]


def create_dungeons(world):
    for dungeon_info in dungeon_table:
        name = dungeon_info['name']
        hint = dungeon_info['hint'] if 'hint' in dungeon_info else name
        font_color = dungeon_info['font_color'] if 'font_color' in dungeon_info else 'White'

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

        world.load_regions_from_json(dungeon_json)
        world.dungeons.append(Dungeon(world, name, hint, font_color))
