from Rom import *

def get_actor_list(rom, actor_func):
    actors = {}
    scene_table_vanilla = 0x00B71440
    scene_table_mq = 0x00BA0BB0 # for MQ
    for scene in range(0x00, 0x65):
        scene_table = scene_table_vanilla
        scene_data = rom.read_int32(scene_table + (scene * 0x14))
        actors.update(scene_get_actors(rom, actor_func, scene_data, scene))
    return actors

def process_scenes(rom):
    scene_table_vanilla = 0x00B71440
    scenes = []
    for i in range(0x00, 0x65):
        scene_table = scene_table_vanilla
        scene_data = rom.read_int32(scene_table + (i * 0x14))
        scene = process_scene(rom, scene_data, i)
        scenes.append(scene)
        print(f"{i} {scene}")
    return scenes

def process_scene(rom, scene_data, scene_id):
    # Start by checking if we have any alternate scene headers
    alt_headers = []
    command = 0
    offset = scene_data
    keep_id = 0
    while command != 0x14: #0x14 = end header
        command = rom.read_byte(offset)
        if command == 0x18: # Alternate header list
            header_list_start = offset + (rom.read_int32(offset + 4) & 0x00FFFFFF)
            for i in range(0, 3):
                alt_header = rom.read_int32(header_list_start + 4*i) & 0x00FFFFFF
                alt_headers.append(offset + alt_header)
        if command == 0x07: # Special object
            keep_id = rom.read_int16(offset + 6)
        offset += 8 

    command = 0
    offset = scene_data
    # Process the rooms
    room_list = []
    while command != 0x14:
        command = rom.read_byte(offset)
        if command == 0x04: # room list
            room_count = rom.read_byte( offset + 1)
            room_table_offset = scene_data + (rom.read_int32(offset + 4) & 0x00FFFFFF)
            for i in range(0, room_count):
                room_start = rom.read_int32(room_table_offset + 8*i)
                room_end = rom.read_int32(room_table_offset + 4 + 8*i)
                process_room(rom, room_list, room_start, room_start, scene_id, i, 0, keep_id)
                # Read the room list
        offset += 8

    return room_list

def process_room(rom, room_list, room_base, room_data, scene_id, room_id, setup, keep_id):
    command = 0
    offset = room_data

    # Check for room alt headers
    if setup == 0:
        while command != 0x14:
            command = rom.read_byte(offset)
            if command == 0x18: # Alternate header list
                header_list_start = offset + (rom.read_int32(offset + 4) & 0x00FFFFFF)
                for i in range(0, 3):
                    alt_header = rom.read_int32(header_list_start + 4*i) & 0x00FFFFFF
                    if alt_header != 0:
                        process_room(rom, room_list, room_base, room_data + alt_header, scene_id, room_id, i+1, keep_id)
                break
            offset += 8
    # Process the room
    command = 0
    offset = room_data
    room_actors = {}
    object_list = []
    while command != 0x14:
        command = rom.read_byte(offset)
        if command == 0x01: # Actor List
            num_actors = rom.read_byte(offset + 1)
            actor_list_start = room_base + (rom.read_int32(offset + 4) & 0x00FFFFFF)
            # Read the actor list
            for i in range(0, num_actors):
                actor_addr = actor_list_start + (i * 0x10)
                actor_bytes = rom.read_bytes(actor_addr, 0x10)
                room_actors[actor_addr] = actor_bytes
        if command == 0x0B: # Object List
            object_count = rom.read_byte(offset + 1)
            object_table_offset = room_base + (rom.read_int32(offset + 4) & 0x00FFFFFF)
            for i in range(0, object_count):
                object_list.append(rom.read_int16(object_table_offset + 2*i))
        offset += 8

    room_list.append((scene_id, room_id, setup, room_data, room_actors, object_list, keep_id))
                

def scene_get_actors(rom, actor_func, scene_data, scene, alternate=None, setup_num=0):
    actors = {}
    scene_start = alternate if alternate else scene_data
    command = 0
    while command != 0x14: # 0x14 = end header
        command = rom.read_byte(scene_data)
        if command == 0x04: #room list
            room_count = rom.read_byte(scene_data + 1)
            room_list = scene_start + (rom.read_int32(scene_data + 4) & 0x00FFFFFF)
            for room_id in range(0, room_count):
                room_data = rom.read_int32(room_list)
                actors, objects = room_get_actors(rom, actor_func, room_data, scene, room_id, setup_num)
                actors.update(actors)
                room_list = room_list + 8
        if command == 0x18: # Alternate header list
            header_list = scene_start + (rom.read_int32(scene_data + 4) & 0x00FFFFFF)
            for alt_id in range(0,3):
                alt_header_addr = rom.read_int32(header_list) & 0x00FFFFFF
                if alt_header_addr != 0 and not alternate:
                    header_data = scene_start + alt_header_addr
                    actors.update(scene_get_actors(rom, actor_func, header_data, scene, scene_start, alt_id+1))
                header_list = header_list + 4

        scene_data = scene_data + 8
    return actors

def room_get_actors(rom, actor_func, room_data, scene, room_id, setup_num, alternate=None):
    actors = {}
    
    objects = []
    room_start = alternate if alternate else room_data
    command = 0
    while command != 0x14: # 0x14 = end header
        command = rom.read_byte(room_data)
        if command == 0x01: # actor list
            actor_count = rom.read_byte(room_data + 1)
            actor_list = room_start + (rom.read_int32(room_data + 4) & 0x00FFFFFF)
            for _ in range(0, actor_count):
                actor_id = rom.read_int16(actor_list)
                entry = actor_func(rom, actor_id, actor_list, scene, room_id, setup_num, _)
                if entry:
                    actors[actor_list] = entry
                actor_list = actor_list + 16
        if command == 0x0B: # object list
            object_count = rom.read_byte(room_data + 1)
            object_list_offset = room_start + (rom.read_int32(room_data + 4) & 0x00FFFFFF)
            for i in range(0, object_count):
                objects.append(rom.read_int16(object_list_offset + i*2))
        if command == 0x18: # Alternate header list
            header_list = room_start + (rom.read_int32(room_data + 4) & 0x00FFFFFF)
            for alt_id in range(0,3):
                alt_header_addr = rom.read_int32(header_list) & 0x00FFFFFF
                if alt_header_addr != 0 and not alternate and setup_num == alt_id + 1:
                    header_data = room_start + alt_header_addr
                    actors.update(room_get_actors(rom, actor_func, header_data, scene,room_id, alt_id+1, room_start))
                header_list = header_list + 4
        room_data = room_data + 8
    return actors, objects

def get_wonderitems(rom):
    def get_wonderitems_func(rom, actor_id, actor, scene, room_id, setup_num, actor_num):
        if actor_id == 0x112:
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_wonderitem(rom.read_bytes(actor, 16)))

    return get_actor_list(rom, get_wonderitems_func)

def get_pots(rom):
    def get_pot_func(rom, actor_id, actor, scene, room_id, setup_num, actor_num):
        if actor_id == 0x111:
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_pot(rom.read_bytes(actor, 16)))
    return get_actor_list(rom, get_pot_func)

def get_crates(rom):
    def get_crate_func(rom, actor_id, actor, scene, room_id, setup_num, actor_num):
        if actor_id == 0x01A0:
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_crate(rom.read_bytes(actor, 16)))
    return get_actor_list(rom, get_crate_func)

def get_small_crates(rom):
    def get_smallcrate_func(rom, actor_id, actor, scene, room_id, setup_num, actor_num):
        if actor_id == 0x0110:
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_small_crate(rom.read_bytes(actor, 16)))
    return get_actor_list(rom, get_smallcrate_func)

def get_flying_pots(rom):
    def get_flyingpot_func(rom, actor_id, actor,scene,room_id,setup_num,actor_num):
        if actor_id == 0x11D:
            pot = (scene, room_id, setup_num, actor_num, scenes[scene], process_flying_pot(rom.read_bytes(actor, 16)))
            return pot
    return get_actor_list(rom, get_flyingpot_func)

def get_empty_and_fairy_pots(rom):
    def get_pot_func(rom, actor_id, actor, scene, room_id, setup_num, actor_num):
        if actor_id == 0x111:
            pot = (scene, room_id, setup_num, actor_num, scenes[scene], process_pot(rom.read_bytes(actor, 16)))
            if pot[5]["item_id"] == "Empty" or pot[5]["item_id"] == "Flexible (Fairy)":
                return pot
    return get_actor_list(rom, get_pot_func)

wondertypes = [
    "MULTITAG_FREE",
    "TAG_POINT_FREE",
    "PROXIMITY_DROP",
    "INTERACT_SWITCH",
    "UNUSED",
    "MULTITAG_ORDERED",
    "TAG_POINT_ORDERED",
    "PROXIMITY_SWITCH",
    "BOMB_SOLDIER",
    "ROLL_DROP"
]

scenes = [
    "Inside the Deku Tree",
    "Dodongo's Cavern",
    "Inside Jabu-Jabu's Belly",
    "Forest Temple",
    "Fire Temple",
    "Water Temple",
    "Spirit Temple",
    "Shadow Temple",
    "Bottom of the Well",
    "Ice Cavern",
    "Ganon's Tower",
    "Gerudo Training Ground",
    "Thieves' Hideout",
    "Inside Ganon's Castle",
    "Ganon's Tower (Collapsing)",
    "Inside Ganon's Castle (Collapsing)",
    "Treasure Box Shop",
    "Gohma's Lair",
    "King Dodongo's Lair",
    "Barinade's Lair",
    "Phantom Ganon's Lair",
    "Volvagia's Lair",
    "Morpha's Lair",
    "Twinrova's Lair & Nabooru's Mini-Boss Room",
    "Bongo Bongo's Lair",
    "Ganondorf's Lair",
    "Tower Collapse Exterior",
    "Market Entrance (Child - Day)",
    "Market Entrance (Child - Night)",
    "Market Entrance (Ruins)",
    "Back Alley (Child - Day)",
    "Back Alley (Child - Night)",
    "Market (Child - Day)",
    "Market (Child - Night)",
    "Market (Ruins)",
    "Temple of Time Exterior (Child - Day)",
    "Temple of Time Exterior (Child - Night)",
    "Temple of Time Exterior (Ruins)",
    "Know-It-All Brothers' House",
    "House of Twins",
    "Mido's House",
    "Saria's House",
    "Carpenter Boss's House",
    "Back Alley House (Man in Green)",
    "Bazaar",
    "Kokiri Shop",
    "Goron Shop",
    "Zora Shop",
    "Kakariko Potion Shop",
    "Market Potion Shop",
    "Bombchu Shop",
    "Happy Mask Shop",
    "Link's House",
    "Back Alley House (Dog Lady)",
    "Stable",
    "Impa's House",
    "Lakeside Laboratory",
    "Carpenters' Tent",
    "Gravekeeper's Hut",
    "Great Fairy's Fountain (Upgrades)",
    "Fairy's Fountain",
    "Great Fairy's Fountain (Spells)",
    "Grottos",
    "Grave (Redead)",
    "Grave (Fairy's Fountain)",
    "Royal Family's Tomb",
    "Shooting Gallery",
    "Temple of Time",
    "Chamber of the Sages",
    "Castle Hedge Maze (Day)",
    "Castle Hedge Maze (Night)",
    "Cutscene Map",
    "Dampé's Grave & Windmill",
    "Fishing Pond",
    "Castle Courtyard",
    "Bombchu Bowling Alley",
    "Ranch House & Silo",
    "Guard House",
    "Granny's Potion Shop",
    "Ganon's Tower Collapse & Battle Arena",
    "House of Skulltula",
    "Spot 00 - Hyrule Field",
    "Spot 01 - Kakariko Village",
    "Spot 02 - Graveyard",
    "Spot 03 - Zora's River",
    "Spot 04 - Kokiri Forest",
    "Spot 05 - Sacred Forest Meadow",
    "Spot 06 - Lake Hylia",
    "Spot 07 - Zora's Domain",
    "Spot 08 - Zora's Fountain",
    "Spot 09 - Gerudo Valley",
    "Spot 10 - Lost Woods",
    "Spot 11 - Desert Colossus",
    "Spot 12 - Gerudo's Fortress",
    "Spot 13 - Haunted Wasteland",
    "Spot 15 - Hyrule Castle",
    "Spot 16 - Death Mountain Trail",
    "Spot 17 - Death Mountain Crater",
    "Spot 18 - Goron City",
    "Spot 20 - Lon Lon Ranch",
    "Ganon's Castle Exterior",

]

dropTable = [
    "NUTS", "HEART_PIECE", "MAGIC_LARGE", "MAGIC_SMALL",
    "HEART", "ARROWS_SMALL", "ARROWS_MEDIUM", "ARROWS_LARGE",
    "RUPEE_GREEN", "RUPEE_BLUE", "RUPEE_RED", "FLEXIBLE"
]

def process_wonderitem(actor_bytes):
    variable = (actor_bytes[14] << 8) + actor_bytes[15]
    type = (variable >> 0xB) & 0x1F
    drop = ((variable >> 6) & 0x1F)
    if(drop < 12):
        drop = dropTable[drop]
    else:
        drop = "Random " + str(drop)
    damage_type = None
    if type == 3: # Interact Switch, get damage type
        damage_type = (actor_bytes[12] << 8) + actor_bytes[13]
    return {
        "type":type,
        "type_string":wondertypes[type],
        "drop": drop,
        "damage_type": damage_type
    }

def process_flying_pot(actor_bytes):
    variable = (actor_bytes[14] << 8) + actor_bytes[15]
    drop = (variable & 0xFF00) >> 8
    flag = (variable & 0x003F)
    return {
        "variable": hex(variable),
        "drop": hex(drop),
        "flag": hex(flag)
    }

def process_small_crate(actor_bytes):
    variable = (actor_bytes[14] << 8) + actor_bytes[15]
    item_id = variable & 0x1F
    item_dict = {
        0x00: "Green Rupee",
        0x01: "Blue Rupee",
        0x02: "Red Rupee",
        0x03: "Recovery Heart",
        0x04: "Bombs (5)",
        0x05: "Arrows (1)",
        0x08: "Arrows (5)",
        0x09: "Arrows (10)",
        0x0A: "Arrows (30)",
        0x0B: "Bombs (5)",
        0x0C: "Deku Nuts (5)",
        0x0D: "Deku Sticks (1)",
        0x0E: "Magic Jar (Small)",
        0x0F: "Magic Jar (Large)",
        0x10: "Deku Seeds (5)",
        0x11: "Small Key",
        0x12: "Flexible (Fairy)",
        0x13: "Huge Rupee",
        0x14: "Purple Rupee",
        0x15: "Deku Shield",
        0x1F: "Empty",
    }
    if not item_id in item_dict.keys():
        item_id = None
    return{
        "variable": hex(variable),
        "item_id": item_dict[item_id]
    }
def process_crate(actor_bytes):
    variable = (actor_bytes[14] << 8) + actor_bytes[15]
    rx = (actor_bytes[8] << 8) + actor_bytes[9]
    # for crates, item dropped iz in Rx
    item_id = (rx & 0xFF)
    item_dict = {
        0x00: "Green Rupee",
        0x01: "Blue Rupee",
        0x02: "Red Rupee",
        0x03: "Recovery Heart",
        0x04: "Bombs (5)",
        0x05: "Arrows (1)",
        0x08: "Arrows (5)",
        0x09: "Arrows (10)",
        0x0A: "Arrows (30)",
        0x0B: "Bombs (5)",
        0x0C: "Deku Nuts (5)",
        0x0D: "Deku Sticks (1)",
        0x0E: "Magic Jar (Small)",
        0x0F: "Magic Jar (Large)",
        0x10: "Deku Seeds (5)",
        0x11: "Small Key",
        0x12: "Flexible (Fairy)",
        0x13: "Huge Rupee",
        0x14: "Purple Rupee",
        0x15: "Deku Shield",
        0x1F: "Empty",
        0xFF: "Empty",
    }
    if not item_id in item_dict.keys():
        item_id = None

    actor_dict = {
        "variable": hex(variable),
        "rx": hex(rx),
        "item_id": item_dict[item_id],
        "skulltula": (variable & 0x8000) == 0
    }
    if actor_dict["skulltula"]:
        actor_dict["skulltula_flag"] = variable & 0xFF
    return actor_dict

def process_pot(actor_bytes):
    variable = (actor_bytes[14] << 8) + actor_bytes[15]
    item_id = variable & 0x1F
    item_dict = {
        0x00: "Green Rupee",
        0x01: "Blue Rupee",
        0x02: "Red Rupee",
        0x03: "Recovery Heart",
        0x04: "Bombs (5)",
        0x05: "Arrows (1)",
        0x08: "Arrows (5)",
        0x09: "Arrows (10)",
        0x0A: "Arrows (30)",
        0x0B: "Bombs (5)",
        0x0C: "Deku Nuts (5)",
        0x0D: "Deku Sticks (1)",
        0x0E: "Magic Jar (Small)",
        0x0F: "Magic Jar (Large)",
        0x10: "Deku Seeds (5)",
        0x11: "Small Key",
        0x12: "Flexible (Fairy)",
        0x13: "Huge Rupee",
        0x14: "Purple Rupee",
        0x15: "Deku Shield",
        0x1F: "Empty",
    }
    if not item_id in item_dict.keys():
        item_id = None
    return{
        "variable": hex(variable),
        "item_id": item_dict[item_id]
    }

def get_overlay_table_entry(rom, actor_id):
    base = 0x00B5E490 + actor_id * 0x20
    vrom_start = rom.read_int32(base)
    vrom_end = rom.read_int32(base + 4)
    vram_start = rom.read_int32(base + 8)
    vram_end = rom.read_int32(base + 12)
    file_ram = rom.read_int32(base + 16)
    init_addr = rom.read_int32(base + 20)
    
    if vram_start:
        init_vrom = init_addr - vram_start + vrom_start
    else:
        init_vrom = 0xA87000 + init_addr - 0x800110A0
        
    init_object = rom.read_int16(init_vrom + 8)
    return {
        'vrom_start': vrom_start,
        'vrom_end': vrom_end,
        'vram_start': vram_start,
        'vram_end': vram_end,
        'file_ram': file_ram,
        'init_addr': init_addr,
        'init_object':init_object
    }


def get_bad_actors(rom):
    scenes_data = process_scenes(rom)

    bad_actors = []
    keep_list = [1,2,3]
    for scene in scenes_data:
        for room in scene:
            scene_id, room_id, setup, room_data, room_actors, object_list, keep_id = room
            i = 0
            for actor in room_actors.keys():
                actor_id = rom.read_int16(actor)
                if actor_id != 0xFFFF: # Ignore any actors we've already patched out
                    overlay_entry = get_overlay_table_entry(rom, actor_id) # Read the entry from the overlay table
                    init_obj = overlay_entry['init_object']
                    # Pots are dumb so handle them seperately
                    if actor_id == 0x111:
                        if keep_id == 0x0003: # Dungeon pot
                            init_obj = 0x0003
                        else:
                            init_obj = 0x12C # Overworld pots use their own object

                    # ignore ganons tower rubble
                    if actor_id == 0x0174:
                        continue

                    # Grass is also dumb
                    if actor_id == 0x0125:
                        params = rom.read_int16(actor + 14)
                        grass_obj_ids = [2, 0x012B, 0x012B]
                        init_obj = grass_obj_ids[params & 0x0003]

                    if init_obj not in object_list and init_obj != keep_id and init_obj != 1 and init_obj <= 0x0191: # Check if the init object is in the rooms data
                        bad_actors.append((actor, scenes[scene_id], scene_id, room_id, setup, i, actor_id, overlay_entry['init_object']))
                    i += 1
    for actor in bad_actors:
        print(actor)
    return bad_actors


#rom = Rom("ZOOTDEC.z64")
#get_bad_actors(rom)
#rom = Rom("zeloot_mqdebug.z64")
#pots = get_crates(rom)

#for pot in pots:
#    print(str(pot) + ": " + str(pots[pot]))

#rom = Rom("../zeloot_mqdebug.z64")
#wonderitems = get_wonderitems(rom)

#for wonderitem in wonderitems:
    #print(str(wonderitem) + ": " + str(wonderitems[wonderitem]))
