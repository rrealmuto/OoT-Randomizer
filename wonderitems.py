from Rom import *
def get_actor_list(rom, actor_func):
    actors = {}
    scene_table = 0x00B71440
    for scene in range(0x00, 0x65):
        scene_data = rom.read_int32(scene_table + (scene * 0x14));
        actors.update(scene_get_actors(rom, actor_func, scene_data, scene))
    return actors


def scene_get_actors(rom, actor_func, scene_data, scene, alternate=None, processed_rooms=None, setup_num=0):
    if processed_rooms == None:
        processed_rooms = []
    actors = {}
    scene_start = alternate if alternate else scene_data
    command = 0
    while command != 0x14: # 0x14 = end header
        command = rom.read_byte(scene_data)
        if command == 0x04: #room list
            room_count = rom.read_byte(scene_data + 1)
            room_list = scene_start + (rom.read_int32(scene_data + 4) & 0x00FFFFFF)
            for room_id in range(0, room_count):
                room_data = rom.read_int32(room_list);

                if not room_data in processed_rooms:
                    actors.update(room_get_actors(rom, actor_func, room_data, scene, room_id, setup_num))
                    processed_rooms.append(room_data)
                room_list = room_list + 8
        if command == 0x18: # Alternate header list
            header_list = scene_start + (rom.read_int32(scene_data + 4) & 0x00FFFFFF)
            for alt_id in range(0,3):
                header_data = scene_start + (rom.read_int32(header_list) & 0x00FFFFFF)
                if header_data != 0 and not alternate:
                    actors.update(scene_get_actors(rom, actor_func, header_data, scene, scene_start, processed_rooms, alt_id+1))
                header_list = header_list + 4

        scene_data = scene_data + 8
    return actors

def room_get_actors(rom, actor_func, room_data, scene, room_id, setup_num, alternate=None):
    actors = {}
    room_start = alternate if alternate else room_data
    command = 0
    while command != 0x14: # 0x14 = end header
        command = rom.read_byte(room_data)
        if command == 0x01: # actor list
            actor_count = rom.read_byte(room_data + 1)
            actor_list = room_start + (rom.read_int32(room_data + 4) & 0x00FFFFFF)
            for _ in range(0, actor_count):
                actor_id = rom.read_int16(actor_list)
                entry = actor_func(rom, actor_id, actor_list, scene, room_id, setup_num)
                if entry:
                    actors[actor_list] = entry
                actor_list = actor_list + 16
        if command == 0x18: # Alternate header list
            header_list = room_start + (rom.read_int32(room_data + 4) & 0x00FFFFFF)
            for alt_id in range(0,3):
                header_data = room_start + (rom.read_int32(header_list) & 0x00FFFFFF)
                if header_data != 0 and not alternate:
                    actors.update(room_get_actors(rom, actor_func, header_data, scene,room_id, alt_id+1, room_start))
                header_list = header_list + 4
        room_data = room_data + 8
    return actors

def get_wonderitems(rom):
    def get_wonderitems_func(rom, actor_id, actor, scene, room_id, setup_num):
        if actor_id == 0x112:
            return (scene, room_id, setup_num, process_wonderitem(rom.read_bytes(actor, 16)))

    return get_actor_list(rom, get_wonderitems_func)

def process_wonderitem(actor_bytes):
    variable = (actor_bytes[14] << 8) + actor_bytes[15]
    type = (variable >> 0xB) & 0x1F
    return {
        "type":type
    }

rom = Rom("ZOOTDEC.z64")
wonderitems = get_wonderitems(rom)

for wonderitem in wonderitems:
    print(str(wonderitem) + ": " + str(wonderitems[wonderitem]))