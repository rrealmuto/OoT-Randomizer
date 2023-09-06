# Literally the stupidest idea ever and I love it

from Rom import *
from ProcessActors import get_actor_list, scenes
import random

def process_brown_boulder(actor_bytes):
    return {
        "type": "brown",
        "switch": (actor_bytes[15] & 0x3F)
    }

def process_bronze_boulder(actor_bytes):
    return {
        "type": "bronze",
        "switch": (actor_bytes[15] & 0x3F)
    }

def process_silver_boulder(actor_bytes):
    var = (actor_bytes[14] << 8) | (actor_bytes[15])
    return {
        "type": "silver",
        "switch": ((var >> 0xA) & 0x3C) | ((var >> 6) & 3)
    }

def process_red_ice(actor_bytes):
    return {
        "type": "red",
        "switch": (actor_bytes[15] & 0x3F)
    }

def process_heavyblock(actor_bytes):
    return {
        "type": "heavyblock",
        "switch": (actor_bytes[14] & 0x3F)
    }

def get_boulder_shuffle_actors(rom):
    def get_boulder_shuffle_actors_func(rom, actor_id, actor, scene, room_id, setup_num, actor_num):
        if actor_id == 0x0127: # Brown bombable boulder
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_brown_boulder(rom.read_bytes(actor, 16)))
        elif actor_id == 0x01D2: #Bronze (hammer) boulder
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_bronze_boulder(rom.read_bytes(actor, 16)))
        elif actor_id == 0x014E: #Silver rock (str2)
            actor_bytes = rom.read_bytes(actor, 16)
            if actor_bytes[15] & 0x0001: # is a big rock and not a little one
                return (scene, room_id, setup_num, actor_num, scenes[scene], process_silver_boulder(rom.read_bytes(actor, 16)))
        #elif actor_id == 0x0092: #Heavy Block (str3 pillar)
        #    return (scene, room_id, setup_num, actor_num, scenes[scene], process_heavyblock(rom.read_bytes(actor, 16)))
        elif actor_id == 0x00EF: # Red Ice
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_red_ice(rom.read_bytes(actor, 16)))
    return get_actor_list(rom, get_boulder_shuffle_actors_func)

def convert_brown_boulder(actor):
    return (0x0127, actor[5]['switch'] & 0x3F)
def convert_bronze_boulder(actor):
    return (0x01D2, actor[5]['switch'] & 0x3F)
def convert_silver_boulder(actor):
    switch = actor[5]['switch']
    var = ((switch & 0x3C) << 0xA) | ((switch & 3) << 6)
    var = var | 0x01
    return  (0x014E, var)
def convert_red_ice(actor):
    return (0x00EF, actor[5]['switch'])
def convert_heavyblock(actor):
    return

convert = {
    "brown": convert_brown_boulder,
    "bronze": convert_bronze_boulder,
    "silver": convert_silver_boulder,
    "red": convert_red_ice,
    "heavyblock": convert_heavyblock
}

def shuffle_boulders(rom: Rom):
    actors = get_boulder_shuffle_actors(rom)
    target_types = [actors[actor][5]['type'] for actor in actors]
    random.shuffle(target_types)
    actor_addresses = list(actors)
    for i in range(0, len(actor_addresses)):
        addr = actor_addresses[i]
        actor = actors[addr]
        curr_actor_type = actors[addr][5]['type']
        actors[addr][5]['newtype'] = target_types[i]
        (id, var) = convert[target_types[i]](actor)
        rom.write_int16(addr, id)
        rom.write_int16(addr + 14, var)

    for actor in actors:
        print(actors[actor])

#rom = Rom("ZOOTDEC.z64")
#shuffle_boulders(rom)