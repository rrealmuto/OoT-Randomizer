from __future__ import annotations
from ast import literal_eval
import random
from ProcessActors import *
from EnemizerList import *
from World import World


def get_rom_enemies(scenes: list[Scene], rom: Rom):
    rom_enemies: dict[tuple[int,int,int,int],Actor] = {}
    for scene in scenes:
        for room in scene.rooms:
            for setup in room.setups:
                actors = room.setups[setup].actors
                for i in range(0, len(actors)):
                    filter = None
                    if actors[i].id in enemy_filters.keys():
                        filter = enemy_filters[actors[i].id]
                    if(filter is None or filter(actors[i])):
                        rom_enemies[(scene.id, room.id,setup,i)] = (actors[i])
    return rom_enemies

def set_enemies(worlds: list[World]):
    for world in worlds:
        world.enemy_list = build_enemylist(world)

def build_enemylist(world: World) -> dict[tuple[int,int,int,int], EnemyLocation]:
    
    location_specific_enemy_logic = {}
    with open(os.path.join(data_path("EnemizerWorld"), "Location Specific Enemy Logic.json"), 'r') as f:
        location_specific_enemy_logic = json.loads(f.read())
    
    enemy_list: dict[tuple[int,int,int,int], EnemyLocation] = {}
    overworld_enemy_list = base_enemy_list.copy()
    update_location_specific_logic(overworld_enemy_list, location_specific_enemy_logic["Overworld"])
    enemy_list.update(overworld_enemy_list)

    mq_dungeons = [dungeon for dungeon in world.dungeon_mq if world.dungeon_mq[dungeon] == True]
    vanilla_dungeons = [dungeon for dungeon in world.dungeon_mq if world.dungeon_mq[dungeon] == False]

    for dungeon in mq_dungeons:
        if dungeon in mq_dungeon_enemies.keys():
            dungeon_enemies = mq_dungeon_enemies[dungeon].copy()
            update_location_specific_logic(dungeon_enemies, location_specific_enemy_logic["MQ Dungeons"][dungeon])
            enemy_list.update(dungeon_enemies)

    for dungeon in vanilla_dungeons:
        if dungeon in vanilla_dungeon_enemies.keys():
            dungeon_enemies = vanilla_dungeon_enemies[dungeon].copy()
            update_location_specific_logic(dungeon_enemies, location_specific_enemy_logic["Vanilla Dungeons"][dungeon])
            enemy_list.update(dungeon_enemies)

    return enemy_list

def update_location_specific_logic(enemy_list: dict[tuple[int,int,int,int], EnemyLocation], location_specific_enemy_logic: dict[str, str]):
    for location in location_specific_enemy_logic.keys():
        location_tuple = literal_eval(location)
        enemy_list[location_tuple].location_specific_enemy_logic = location_specific_enemy_logic[location]

def shuffle_enemies(worlds: list[World]):
    for world in worlds:
        world.shuffled_enemies = _shuffle_enemies(world,world.enemy_list)

        # Process enemy logic at this time
        for enemy_key in world.shuffled_enemies:
            enemy_obj = world.shuffled_enemies[enemy_key][0]
            # Check if we should use a location specific logic rule for the enemy at this location
            if enemy_obj.name in world.enemy_list[enemy_key].location_specific_enemy_logic:
                # Use the location specific kill rule for this enemy type
                world.enemy_list[enemy_key].kill_rule = world.parser.parse_rule(world.enemy_list[enemy_key].location_specific_enemy_logic[enemy_obj.name])
                world.enemy_list[enemy_key].drop_rule = None
            else:
                # Use the generic kill rule
                world.enemy_list[enemy_key].kill_rule = world.parser.parse_rule(enemy_obj.kill_logic)
                if enemy_obj.drop_logic:
                    world.enemy_list[enemy_key].drop_rule = world.parser.parse_rule(enemy_obj.drop_logic)
                else:
                    world.enemy_list[enemy_key].drop_rule = None
        # Enemies by scene/room
        scene_enemies = {}
        for key in world.shuffled_enemies:
            scene, room, setup, index = key
            if scene not in scene_enemies.keys():
                scene_enemies[scene] = {}
            if room not in scene_enemies[scene].keys():
                scene_enemies[scene][room] = {}
            if setup not in scene_enemies[scene][room].keys():
                scene_enemies[scene][room][setup] = {}
            scene_enemies[scene][room][setup][key] = world.shuffled_enemies[key]
        world.enemies_by_scene = scene_enemies

def _shuffle_enemies(world: World, enemy_list: dict[tuple[int,int,int,int], EnemyLocation]) -> dict[tuple[int,int,int,int], tuple[int,bool]]:
    to_shuffle: dict[tuple[int,int,int,int], EnemyLocation] = enemy_list.copy()

    shuffled: dict[tuple[int,int,int,int], tuple[Enemy,bool]] = {}
    # Handle plandoed enemies
    for plando_enemy_key, enemy_name in world.distribution.enemies.items():
        enemy = enemies_by_name[enemy_name]

        shuffled[plando_enemy_key] = (enemy, True)
        del to_shuffle[plando_enemy_key]

    # Remove broken actors if fix_broken_actors is off
    if not world.settings.fix_broken_actors:
        for enemy in enemy_list:
            if enemy in to_shuffle.keys() and to_shuffle[enemy].is_broken_actor:
                del to_shuffle[enemy]

    if world.settings.enemizer == 'on':
        for enemy_key in to_shuffle:
            shuffle_enemy = to_shuffle[enemy_key]
            if type(shuffle_enemy) is EnemyLocation: # EnemyLocation with type restrictions
                restriction = shuffle_enemy.restrictions
                meets_enemy_restrictions = shuffle_enemy.meets_enemy_restrictions
                disallowed_enemies = shuffle_enemy.disallowed_enemies
                allowed_enemies = shuffle_enemy.explicit_allowed_enemies
                #enemy_type = enemy_type.id
            else: # Just an enemy ID
                #enemy_type = to_shuffle[enemy_key]
                restriction = []
                meets_enemy_restrictions = []
                disallowed_enemies = []
                allowed_enemies = []
            enemy_choices = list(get_restricted_enemy_types(enemy_actor_types, restriction, meets_enemy_restrictions, disallowed_enemies, allowed_enemies))
            weights = [enemy.weight for enemy in enemy_choices]
            enemy = random.choices(enemy_choices, weights=weights)[0]

            shuffled[enemy_key] = (enemy, True)

    return shuffled

# Get a list of allowed enemy types based on the restrictions passed in
# enemy_actor_types - enemy dict
# restrictions - list of location's restrictions. Enemies must be tagged with the same restriction in order to be included
# meets_enemy_restrictions - list of the enemy restrictions that the location meets. Enemies with ENEMY_RESTRICTIONs will not be allowed unless the location is tagged with that restriction
# Disallowed enemies - list of enemy actor ID's to explicitly exclude from a location
def get_restricted_enemy_types(enemy_actor_types: list[Enemy], location_restrictions: list[LOCATION_RESTRICTION], meets_enemy_restrictions: list[ENEMY_RESTRICTION], disallowed_enemies: list[str], allowed_enemies: list[str]):
    #restricted_enemy_actor_types: dict[int,Enemy] = {}
    restricted_enemy_actor_types: list[Enemy] = []
    for enemy in enemy_actor_types:
        meets_restrictions = True
        
        if enemy.name in allowed_enemies:
            meets_restrictions = True
            restricted_enemy_actor_types.append(enemy)
            continue

        # Check location restrictions against enemy. An enemy must meet all of the locations restrictions in order to be placed
        for restriction in location_restrictions:
            if restriction not in enemy.meets_location_restrictions:
                meets_restrictions = False
                break

        # Check explicitly disallowed enemies
        if enemy.name in disallowed_enemies:
            meets_restrictions = False
            continue
        
        # Check enemy restrictions against location. A location must meet all of the enemy's restrictions in order to be placed
        for required_category in enemy.required_categories:
            if required_category not in meets_enemy_restrictions:
                meets_restrictions = False
                break
        if meets_restrictions:
            restricted_enemy_actor_types.append(enemy)
    return restricted_enemy_actor_types

def patch_enemies(world: World,enemy_list: dict[tuple[int,int,int,int],Actor], shuffled_enemies: dict[tuple[int,int,int,int], tuple[Enemy, bool]], rom: Rom, scene_data: list[Scene], enemizer_on: bool):

    switch_flags_table = []
    skip_raycast_table = []
    if enemizer_on:
        for enemy_key in shuffled_enemies:
            keys = [enemy_key]
            if enemy_key in base_enemy_alts.keys():
                alt = base_enemy_alts[enemy_key]
                if type(alt) is list:
                    keys.extend(base_enemy_alts[enemy_key])
                else:
                    keys.append(base_enemy_alts[enemy_key])
            for key in keys:
                enemy, shuffled = shuffled_enemies[enemy_key]
                if key in enemy_list.keys():
                    enemy_actor = enemy_list[key]
                    if shuffled and enemy_actor.id != 0xFFFF:
                        enemy_actor.rot_x = 0
                        enemy_actor.rot_z = 0
                        enemy_actor.id = enemy.id
                        enemy_actor.var = enemy.var
                        if key in world.enemy_list and type(world.enemy_list[key]) is EnemyLocation:
                            if world.enemy_list[key].patch_func:
                                world.enemy_list[key].patch_func(enemy_actor)
                            if enemy.name in world.enemy_list[key].var_overrides.keys():
                                enemy_actor.var = world.enemy_list[key].var_overrides[enemy.name]
                        rom.write_bytes(enemy_actor.addr, enemy_actor.get_bytes())
                        if key in world.enemy_list and type(world.enemy_list[key]) is EnemyLocation:
                            if world.enemy_list[key].switch_flag >= 0:
                                switch_flags_table.append((key,world.enemy_list[key].switch_flag))
                        if key in world.enemy_list and type(world.enemy_list[key]) is EnemyLocation and world.enemy_list[key].skip_raycast:
                            skip_raycast_table.append(key)
                else:
                    print(f"Missing enemy actor {key}")
    else:
        for enemy_actor_key in enemy_list:
            enemy_actor = enemy_list[enemy_actor_key]
            if enemy_actor_key in world.enemy_list and type(world.enemy_list[enemy_actor_key]) is EnemyLocation:
                if world.enemy_list[enemy_actor_key].patch_func:
                    world.enemy_list[enemy_actor_key].patch_func(enemy_actor)
                    rom.write_bytes(enemy_actor.addr, enemy_actor.get_bytes())
                if world.enemy_list[enemy_actor_key].switch_flag >= 0:
                    switch_flags_table.append((enemy_actor_key,world.enemy_list[enemy_actor_key].switch_flag))

    # Write the switch flags table
    switch_flags_table_bytes = bytearray()
    for flag, switch_flag in switch_flags_table:
        scene, room, setup, id = flag
        id += 1
        subflag = 0
        if scene == 0x3E: # handle grottos separately...
            default = ((setup & 0x1F) << 19) + ((room & 0x0F) << 15) + ((id & 0x7F) << 8) + ((subflag & 0xFF)) #scene_setup = grotto_id
        else:
            default = (setup << 22) + (room << 16) + (id << 8) + (subflag)
        switch_flags_table_bytes.extend(scene.to_bytes(1,'big'))
        switch_flags_table_bytes.extend(bytearray([0,0,0]))
        switch_flags_table_bytes.extend(default.to_bytes(4,'big'))
        switch_flags_table_bytes.extend(switch_flag.to_bytes(1, 'big'))
        switch_flags_table_bytes.extend(bytearray([0,0,0]))
    rom.write_bytes_at_symbol('KILL_SWITCH_TABLE', switch_flags_table_bytes)

    # Write the raycast skip table
    skip_raycast_table_bytes = bytearray()
    for flag in skip_raycast_table:
        scene, room, setup, id = flag
        id += 1
        subflag = 0
        if scene == 0x3E: # handle grottos separately...
            default = ((setup & 0x1F) << 19) + ((room & 0x0F) << 15) + ((id & 0x7F) << 8) + ((subflag & 0xFF)) #scene_setup = grotto_id
        else:
            default = (setup << 22) + (room << 16) + (id << 8) + (subflag)
        skip_raycast_table_bytes.extend(scene.to_bytes(1, 'big'))
        skip_raycast_table_bytes.extend(bytearray([0,0,0]))
        skip_raycast_table_bytes.extend(default.to_bytes(4, 'big'))
    rom.write_bytes_at_symbol('SKIP_RAYCAST_TABLE', skip_raycast_table_bytes)

# Nabooru knuckle enemizer patch function
# Patch the door to work on room clear instead of switch flag
def patch_nabooru_knuckle(rom: Rom, world: World, scene_data: list[Scene]):
    nabooru_transition = scene_data[23].transition_actors[1]
    nabooru_transition.var = 0x40
    rom.write_bytes(nabooru_transition.addr, nabooru_transition.get_bytes())

# Jabu Jabu Shabom Room timer patch
# Disable the timer in enemizer
# Only for vanilla jabu
def patch_jabu_jabu_room_timer(rom: Rom, world: World, scene_data: list[Scene]):
    if not world.dungeon_mq['Jabu Jabus Belly']:
        timer = scene_data[2].rooms[12].setups[0].actors[0]
        timer.var = 0x7878 # Patch the timer to have 2 minute time limit
        rom.write_bytes(timer.addr, timer.get_bytes())

# Patch RestrictionFlags list to allow weapons in adult market
def patch_adult_market(rom: Rom, world: World, scene_data: list[Scene]):
    rom.write_int32(0xB6D2B0 + 0xB4, 0x22000000)

# Patch GTG Boulder Room room timer to use custom behavior
def patch_gtg_boulder_room(rom: Rom, world: World, scene_data: list[Scene]):
    timer = scene_data[0x0B].rooms[2].setups[0].actors[18 if not world.dungeon_mq['Gerudo Training Ground'] else 22]
    timer.rot_z = 1
    rom.write_bytes(timer.addr, timer.get_bytes())

# Add patch funcs here, we'll call them in a loop in patches.py
enemizer_patches = [
    patch_nabooru_knuckle,
    patch_jabu_jabu_room_timer,
    patch_adult_market,
    patch_gtg_boulder_room
]
