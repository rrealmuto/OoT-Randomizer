from __future__ import annotations
from math import ceil
from typing import TYPE_CHECKING

if TYPE_CHECKING:
    from Location import Location
    from World import World


# Loop through all of the locations in the world. Extract ones that use our flag system and 
def build_xflags_from_world(world: World) -> tuple[dict[int, dict[int, int]], list[tuple[Location, tuple[int, int, int], tuple[int, int, int]]]]:
    scene_flags = {}
    alt_list = []
    for i in range(0, 101):
        scene_flags[i] = {}
        for location in world.get_locations():
            if location.scene == i and location.type in ["Freestanding", "Pot", "FlyingPot", "Crate", "SmallCrate", "Beehive", "RupeeTower", "SilverRupee"]:
                default = location.default
                if isinstance(default, list):  # List of alternative room/setup/flag to use
                    primary_tuple = default[0]
                    if len(primary_tuple) == 3: 
                        room, setup, flag = primary_tuple
                        alt_flag = 1
                        primary_tuple = (room, setup, flag, alt_flag)
                    for c in range(1, len(default)):
                        alt = default[c]
                        if len(alt) == 3: 
                            room, setup, flag = alt
                            alt_flag = 1
                            alt = (room, setup, flag, alt_flag)
                        alt_list.append((location, alt, primary_tuple))
                    default = primary_tuple  # Use the first tuple as the primary tuple
                if isinstance(default, tuple):
                    if len(default) == 3: 
                        room, setup, flag = default
                        alt_flag = 1
                    elif len(default) == 4:
                        room, setup, flag, alt_flag = default
                    room_setup = (setup, room)
                    if not room_setup in scene_flags[i].keys():
                        scene_flags[i][room_setup] = []
                    scene_flags[i][room_setup].append((flag, alt_flag))
                        
        if len(scene_flags[i].keys()) == 0:
            del scene_flags[i]
    return scene_flags, alt_list
                
def build_xflag_tables(xflags):
    scene_table = bytearray([0xFF] * 202)
    room_table = bytearray(0)
    room_blob = bytearray(0)
    bits = 0
    for scene in xflags.keys():
        num_room_setups = len(xflags[scene].keys())
        room_table_offset = len(room_table)
        scene_table[scene*2] = (room_table_offset & 0xFF00) >> 8
        scene_table[scene*2 + 1] = (room_table_offset & 0x00FF)
        room_table.append(num_room_setups)
        for setup, room in xflags[scene].keys():
            room_setup = (setup << 6) + room
            room_xflags, room_bits = build_room_xflags(xflags[scene][(setup,room)])
            diff_flags, rlc_flags = encode_room_xflags(room_xflags)
            room_table.append(room_setup)
            room_blob_offset = len(room_blob)
            room_table.append((room_blob_offset & 0xFF00) >> 8)
            room_table.append(room_blob_offset & 0x00FF)
            room_blob.append((bits & 0xFF00) >> 8)
            room_blob.append(bits & 0x00FF)
            room_blob.append(len(rlc_flags))
            room_blob.extend(bytearray(rlc_flags))    
            bits += room_bits
    return scene_table, room_table, room_blob

# Create a 255 byte array representing each actor in the room. Each value in the array is the bit index that will be used for that actor, accounting for sub_ids
# room_locations - list of location (actor_id, sub_id) in the room
def build_room_xflags(room_locations):
    # Loop through every shuffled location in the room
    room_xflags = [0] * 256
    for actor_id, sub_id in room_locations:
        if sub_id > room_xflags[actor_id]:
            room_xflags[actor_id] = sub_id
    bits = 0
    room_xflags2 = [0] * 256
    for i in range(1, 256):
        room_xflags2[i] = bits + room_xflags[i - 1]
        bits += room_xflags[i - 1]
    return room_xflags2, bits

def encode_room_xflags(xflags):
    # Pass 1, differential encoding
    diff_flags = [0]*256
    diff_flags[0] = xflags[0]
    for i in range(1, 256):
        diff_flags[i] = xflags[i] - xflags[i-1]

    # Pass 2, Run length coding
    rlc_flags = []
    curr_token = diff_flags[0]
    curr_token_count = 1
    for i in range(1, 256):
        if diff_flags[i] == curr_token:
            curr_token_count += 1
        else:
            rlc_flags.append(curr_token)
            rlc_flags.append(curr_token_count)
            curr_token = diff_flags[i]
            curr_token_count = 1

    return diff_flags, rlc_flags

# Create a byte array from the scene flag table created by get_collectible_flag_table
def get_collectible_flag_table_bytes(scene_flag_table: dict[int, dict[int, int]]) -> tuple[bytearray, int]:
    num_flag_bytes = 0
    bytes = bytearray()
    bytes.append(len(scene_flag_table.keys()))
    for scene_id in scene_flag_table.keys():
        rooms = scene_flag_table[scene_id]
        room_count = len(rooms.keys())
        bytes.append(scene_id)
        bytes.append(room_count)
        for room in rooms:
            bytes.append(room)
            bytes.append((num_flag_bytes & 0xFF00) >> 8)
            bytes.append(num_flag_bytes & 0x00FF )
            num_flag_bytes += ceil((rooms[room] + 1) / 8)

    return bytes, num_flag_bytes


def get_alt_list_bytes(alt_list: list[tuple[Location, tuple[int, int, int], tuple[int, int, int]]]) -> bytearray:
    bytes = bytearray()
    for entry in alt_list:
        location, alt, primary = entry
        room, scene_setup, flag, subflag = alt
        if location.scene is None:
            continue

        alt_override = (room << 8) + (scene_setup << 14) + flag
        room, scene_setup, flag, subflag = primary
        primary_override = (room << 8) + (scene_setup << 14) + flag
        bytes.append(location.scene)
        bytes.append(0x06)
        bytes.append((alt_override & 0xFF00) >> 8)
        bytes.append(alt_override & 0xFF)
        bytes.append(location.scene)
        bytes.append(0x06)
        bytes.append((primary_override & 0xFF00) >> 8)
        bytes.append(primary_override & 0xFF)
    return bytes
