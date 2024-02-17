import sys
from Rom import *
from PIL import Image, ImageDraw


class Vtx:
    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def from_bytes(bytes: bytearray):
        x = int.from_bytes(bytes[0:2],'big', signed=True)
        y = int.from_bytes(bytes[2:4],'big', signed=True)
        z = int.from_bytes(bytes[4:6],'big', signed=True)
        return Vtx(x,y,z)

class Actor:
    def __init__(self, address: int, actor_bytes: bytearray):
        self.addr = address
        self.id = int.from_bytes(actor_bytes[0:2], 'big')
        self.var = int.from_bytes(actor_bytes[14:16], 'big')
        self.pos: Vtx = Vtx.from_bytes(actor_bytes[2:8])

class RoomSetup:
    def __init__(self, rom: Rom, room_addr: int, header_addr: int):
        # Process the room
        command = 0
        offset = header_addr
        self.actors: list[Actor] = []
        self.objects: list[int] = []
        while command != 0x14:
            command = rom.read_byte(offset)
            if command == 0x01: # Actor List
                num_actors = rom.read_byte(offset + 1)
                actor_list_start = room_addr + (rom.read_int32(offset + 4) & 0x00FFFFFF)
                # Read the actor list
                for i in range(0, num_actors):
                    actor_addr = actor_list_start + (i * 0x10)
                    actor_bytes = rom.read_bytes(actor_addr, 0x10)
                    self.actors.append(Actor(actor_addr, actor_bytes))
            if command == 0x0B: # Object List
                object_count = rom.read_byte(offset + 1)
                object_table_offset = room_addr + (rom.read_int32(offset + 4) & 0x00FFFFFF)
                for i in range(0, object_count):
                    self.objects.append(rom.read_int16(object_table_offset + 2*i))
            offset += 8

        #room_list.append((scene_id, room_id, setup, room_data, room_actors, object_list, keep_id))

class Room:
    def __init__(self, rom: Rom, id: int, rom_addr: int):
        self.id = id
        self.setups: dict[int, RoomSetup] = Room.process_room(rom, rom_addr)
        self.rom_addr = rom_addr
    
    @staticmethod
    def process_room(rom: Rom, rom_addr: int) -> dict[int, RoomSetup]:
        room_list = {}
        Room._process_room(rom, room_list, rom_addr, rom_addr, 0)
        return room_list

    @staticmethod
    def _process_room(rom, room_list, room_base, room_data, setup):
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
                            Room._process_room(rom, room_list, room_base, room_data + alt_header, i+1)
                    break
                offset += 8
        room_list[setup] = RoomSetup(rom, room_base, room_data)

class Poly:
    def __init__(self, poly_bytes: bytearray):
        self.type = int.from_bytes(poly_bytes[0:2], 'big')
        self.vertex = []
        self.vertex.append(int.from_bytes(poly_bytes[2:4], 'big') & 0x1FFF)
        self.vertex.append(int.from_bytes(poly_bytes[4:6], 'big') & 0x1FFF)
        self.vertex.append(int.from_bytes(poly_bytes[6:8], 'big') & 0x1FFF)

class SceneCollision:
    # Read a scene's collision data given the collision command in the scene header
    def __init__(self, rom: Rom, scene, scene_addr: int, collision_command: bytearray):
        # read the collision command
        self.scene = scene
        self.segment_offset = int.from_bytes(collision_command[4:8], 'big') & 0x00FFFFFF
        self.header_bytes = rom.read_bytes(scene_addr + self.segment_offset, 0x28)
        bb_min = self.header_bytes[0:6]
        bb_max = self.header_bytes[6:12]
        self.vertex_count = int.from_bytes(self.header_bytes[12:14], 'big')
        self.vertex_array_offset = int.from_bytes(self.header_bytes[16:20], 'big')
        self.poly_count = int.from_bytes(self.header_bytes[20:22], 'big')
        self.poly_array_offset = int.from_bytes(self.header_bytes[24:28], 'big')
        self.bounding_box_min = Vtx(int.from_bytes(bb_min[0:2],'big',signed=True), int.from_bytes(bb_min[2:4],'big',signed=True), int.from_bytes(bb_min[4:6],'big',signed=True))
        self.bounding_box_max = Vtx(int.from_bytes(bb_max[0:2],'big',signed=True), int.from_bytes(bb_max[2:4],'big',signed=True), int.from_bytes(bb_max[4:6],'big',signed=True))
        self.vertex: list[Vtx] = []
        self.poly: list[Poly] = []
        for i in range(0, self.vertex_count):
            vtx_bytes = rom.read_bytes(scene_addr + (self.vertex_array_offset & 0x00FFFFFF) + (6*i), 6)
            self.vertex.append(Vtx(int.from_bytes(vtx_bytes[0:2],'big',signed=True), int.from_bytes(vtx_bytes[2:4],'big',signed=True), int.from_bytes(vtx_bytes[4:6],'big',signed=True)))
        for i in range(0, self.poly_count):
            poly_bytes = rom.read_bytes(scene_addr + (self.poly_array_offset & 0x00FFFFFF) + (0x10*i), 0x10)
            self.poly.append(Poly(poly_bytes))
        
    def normalize_vtx(self, vtx: Vtx):
        normalized = Vtx(vtx.x, vtx.y, vtx.z)
        normalized.x -= self.min.x
        normalized.x /= self.max.x - self.min.x
        normalized.y -= self.min.y
        normalized.y /= self.max.y - self.min.y
        normalized.z -= self.min.z
        normalized.z /= self.max.z - self.min.z
        return normalized

    def draw_collider_mesh(self):
        # Normalize vertex coordinates using the bounding box

        # Find the real bounding box
        try:
                
            min = Vtx(self.vertex[0].x, self.vertex[0].y, self.vertex[0].z)
            max = Vtx(self.vertex[0].x, self.vertex[0].y, self.vertex[0].z)
            self.min = min
            self.max = max

            for v in self.vertex:
                if v.x < min.x:
                    min.x = v.x
                if v.y < min.y:
                    min.y = v.y
                if v.z < min.z:
                    min.z = v.z

                if v.x > max.x:
                    max.x = v.x
                if v.y > max.y:
                    max.y = v.y
                if v.z > max.z:
                    max.z = v.z

            vertex_normalized = []
            for i in range(0, len(self.vertex)):
                normalized = Vtx(self.vertex[i].x, self.vertex[i].y, self.vertex[i].z)
                normalized.x -= min.x
                normalized.x /= max.x - min.x
                normalized.y -= min.y
                normalized.y /= max.y - min.y
                normalized.z -= min.z
                normalized.z /= max.z - min.z

                vertex_normalized.append(normalized)
            self.vertex_normalized = vertex_normalized

            # Figure out how big to make our image
            x_size = max.x - min.x
            y_size = max.z - min.z
            self.x_size = x_size
            self.y_size = y_size

            im = Image.new('L',(x_size + 1, y_size + 1))
            draw = ImageDraw.Draw(im)

            self.im = im
            self.draw = draw
            # draw each poly
            for p in self.poly:
                # draw lines connecting each vertex
                # 0, 1
                # 1, 2
                # 2, 0
                v0n = self.vertex_normalized[p.vertex[0]]
                v1n = self.vertex_normalized[p.vertex[1]]
                v2n = self.vertex_normalized[p.vertex[2]]
                draw.line([(v0n.x * x_size, v0n.z * y_size), (v1n.x * x_size, v1n.z * y_size)], fill=255)
                draw.line([(v1n.x * x_size, v1n.z * y_size), (v2n.x * x_size, v2n.z * y_size)], fill=255)
                draw.line([(v2n.x * x_size, v2n.z * y_size), (v0n.x * x_size, v0n.z * y_size)], fill=255)
            return im
        except Exception:
            pass
        return None
    
    def draw_room_actors(self,room: Room):
        for actor in room.setups[0].actors:
            if actor.id == 0x0A:
                pos_norm = self.normalize_vtx(actor.pos)
                self.draw.rectangle(((pos_norm.x * self.x_size - 10, pos_norm.z * self.y_size -10), (pos_norm.x * self.x_size + 10, pos_norm.z * self.y_size + 10)), 255, width=10)

class Scene:
    def __init__(self, rom: Rom, id: int, scene_addr: int):
        self.rooms: list[Room] = []
        self.keep_id = 0
        self.id = id
        self.collision = None
        self.process_scene(rom, scene_addr)
        self.rom_addr = scene_addr

    def process_scene(self, rom, scene_data):
        command = 0
        offset = scene_data
        while command != 0x14: #0x14 = end header
            command = rom.read_byte(offset)
            if command == 0x07: # Special object
                self.keep_id = rom.read_int16(offset + 6)
            offset += 8 

        command = 0
        offset = scene_data
        # Process the rooms
        while command != 0x14:
            command = rom.read_byte(offset)
            if command == 0x04: # room list
                room_count = rom.read_byte( offset + 1)
                room_table_offset = scene_data + (rom.read_int32(offset + 4) & 0x00FFFFFF)
                for i in range(0, room_count):
                    room_start = rom.read_int32(room_table_offset + 8*i)
                    room_end = rom.read_int32(room_table_offset + 4 + 8*i)
                    room = Room(rom, i, room_start)
                    self.rooms.append(room)
                    # Read the room list
            if command == 0x03: # collision header
                collision_command = rom.read_bytes(offset, 8)
                self.collision = SceneCollision(rom, self, scene_data, collision_command)
            offset += 8

def get_actor_list(rom, actor_func):
    actors = {}
    scene_table_vanilla = 0x00B71440
    scene_table_mq = 0x00BA0BB0 # for MQ
    for scene in range(0x00, 0x65):
        scene_table = scene_table_vanilla
        scene_data = rom.read_int32(scene_table + (scene * 0x14))
        actors.update(scene_get_actors(rom, actor_func, scene_data, scene))
    return actors

def process_scenes(rom) -> list[Scene]:
    scene_table_vanilla = 0x00B71440
    scenes = []
    for i in range(0x00, 0x65):
        scene_table = scene_table_vanilla
        scene_data = rom.read_int32(scene_table + (i * 0x14))
        scene = Scene(rom, i, scene_data)
        scenes.append(scene)
        print(f"{i} {scene}")
    return scenes

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
    room_list[(scene_id, room_id, setup)] = (room_data, room_actors, object_list, keep_id)
    #room_list.append((scene_id, room_id, setup, room_data, room_actors, object_list, keep_id))
                

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
                actors.update(room_get_actors(rom, actor_func, room_data, scene, room_id, setup_num))
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
        if command == 0x18: # Alternate header list
            header_list = room_start + (rom.read_int32(room_data + 4) & 0x00FFFFFF)
            for alt_id in range(0,3):
                alt_header_addr = rom.read_int32(header_list) & 0x00FFFFFF
                if alt_header_addr != 0 and not alternate and setup_num == alt_id + 1:
                    header_data = room_start + alt_header_addr
                    actors.update(room_get_actors(rom, actor_func, header_data, scene,room_id, alt_id+1, room_start))
                header_list = header_list + 4
        room_data = room_data + 8
    return actors

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

def get_grass(rom):
    def get_grass_func(rom, actor_id, actor, scene, room_id, setup_num, actor_num):
        func = None
        if actor_id == 0x0125: # En_Kusa - Bush/Grass (single)
            func = process_bush
        elif actor_id == 0x0151: # Obj_Mure2 - Rock/Bush circle
            func = process_bush_circle
        else:
            return
        data = func(rom.read_bytes(actor,16))
        if data:
            bush = (scene, room_id, setup_num, actor_num, scenes[scene], data)
            return bush

    return get_actor_list(rom, get_grass_func)

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
    "Hyrule Field",
    "Kakariko Village",
    "Graveyard",
    "Zora's River",
    "Kokiri Forest",
    "Sacred Forest Meadow",
    "Lake Hylia",
    "Zora's Domain",
    "Zora's Fountain",
    "Gerudo Valley",
    "Lost Woods",
    "Desert Colossus",
    "Gerudo's Fortress",
    "Haunted Wasteland",
    "Hyrule Castle",
    "Death Mountain Trail",
    "Death Mountain Crater",
    "Goron City",
    "Lon Lon Ranch",
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

def process_bush(actor_bytes):
    bush_types = {
        0: "Shrub",
        1: "Cuttable Regenerating Grass",
        2: "Cuttable Grass"
    }
    type = actor_bytes[15] & 0x03
    drop_table = actor_bytes[14] & 0xFF
    if type in bush_types.keys():
        return {
            "type": bush_types[type],
            "drop_table": drop_table
        }

def process_bush_circle(actor_bytes):
    type = actor_bytes[15] & 0x0F
    drop_table = actor_bytes[14] & 0xFF
    if type == 0: # Circle of shrubs with on in the middle, random drops
        return {
            "type": "Bush Circle",
            "drop_table": drop_table
        }
    elif type == 1: # Scattered shrubs, random drops
        return {
            "type": "Scattered Bushes",
            "drop_table": drop_table
        }
    else: # Rocks/don't care
        pass
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


def get_bad_actors(rom: Rom, scenes_data: list[Scene]):
    bad_actors = []
    keep_list = [1,2,3]
    for scene in scenes_data:
        for room in scene.rooms:
            for setup in room.setups:
                i = 0
                for actor in room.setups[setup].actors:
                    if actor.id != 0xFFFF: # Ignore any actors we've already patched out
                        overlay_entry = get_overlay_table_entry(rom, actor.id) # Read the entry from the overlay table
                        init_obj = overlay_entry['init_object']
                        # Pots are dumb so handle them seperately
                        if actor.id == 0x111:
                            if scene.keep_id == 0x0003: # Dungeon pot
                                init_obj = 0x0003
                            else:
                                init_obj = 0x12C # Overworld pots use their own object

                        # ignore ganons tower rubble
                        if actor.id == 0x0174:
                            continue

                        # Grass is also dumb
                        if actor.id == 0x0125:
                            params = rom.read_int16(actor.addr + 14)
                            grass_obj_ids = [2, 0x012B, 0x012B]
                            init_obj = grass_obj_ids[params & 0x0003]

                        if init_obj not in room.setups[setup].objects and init_obj != scene.keep_id and init_obj != 1 and init_obj <= 0x0191: # Check if the init object is in the rooms data
                            bad_actors.append((actor.addr, scenes[scene.id], scene.id, room.id, setup, i, actor.id, overlay_entry['init_object']))
                        i += 1
    for actor in bad_actors:
        print(actor)
    return bad_actors

#rom = Rom("ZOOTDEC.z64")

#get_bad_actors(rom)
#scene_info = process_scenes(rom)
#rom = Rom("zeloot_mqdebug.z64")
#pots = get_crates(rom)

#for pot in pots:
#    print(str(pot) + ": " + str(pots[pot]))

if __name__ == "__main__":
    #rom = Rom("ZOOTDEC.z64")
    rom = Rom("ZOOTDEC.z64")
    rom_scenes = process_scenes(rom)
    for scene in rom_scenes:
        im: Image = scene.collision.draw_collider_mesh()
        if im:
            draw = ImageDraw.Draw(im)
            for room in scene.rooms:
                scene.collision.draw_room_actors(room)
            scene.collision.im.save(scenes[scene.id] + ".png", "PNG")
        
    #rom = Rom("../zeloot_mqdebug.z64")
    #wonderitems = get_wonderitems(rom)

    #for wonderitem in wonderitems:
        #print(str(wonderitem) + ": " + str(wonderitems[wonderitem]))
