import random
from ProcessActors import *
from EnemizerList import *
from World import World


def get_rom_enemies(scenes: list[Scene], rom: Rom):
    enemy_list: dict[tuple[int,int,int,int],Actor] = {}
    for scene in scenes:
        for room in scene.rooms:
            for setup in room.setups:
                actors = room.setups[setup].actors
                for i in range(0, len(actors)):
                    if actors[i].id in enemy_actor_types.keys():
                        filter = enemy_actor_types[actors[i].id].filter_func
                        if(filter is None or filter(actors[i])):
                            enemy_list[(scene.id, room.id,setup,i)] = (actors[i])
    return enemy_list

def set_enemies(worlds: list[World]):
    for world in worlds:
        world.enemy_list = build_enemylist(world)

def build_enemylist(world: World):
    enemy_list = base_enemy_list.copy()
    mq_dungeons = [dungeon for dungeon in world.dungeon_mq if world.dungeon_mq[dungeon] == True]
    vanilla_dungeons = [dungeon for dungeon in world.dungeon_mq if world.dungeon_mq[dungeon] == False]
    
    for dungeon in mq_dungeons:
        if dungeon in mq_dungeon_enemies.keys():
            enemy_list.update(mq_dungeon_enemies[dungeon])
    
    for dungeon in vanilla_dungeons:
        if dungeon in vanilla_dungeon_enemies.keys():
            enemy_list.update(vanilla_dungeon_enemies[dungeon])

    return enemy_list

def shuffle_enemies(worlds: list[World]):
    for world in worlds:
        world.shuffled_enemies = _shuffle_enemies(world,world.enemy_list)
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

def _shuffle_enemies(world: World, enemy_list: dict[tuple[int,int,int,int],int | EnemyLocation]) -> dict[tuple[int,int,int,int], tuple[int,bool]]:
    to_shuffle = enemy_list.copy()

    shuffled: dict[tuple[int,int,int,int], tuple[int,bool]] = {}
    # Handle plandoed enemies
    for plando_enemy_key, enemy_name in world.distribution.enemies.items():
        enemy_id = enemies_by_name[enemy_name]
        enemy = enemy_actor_types[enemy_id]
        if type(enemy) is EnemyWithOpts:
            for opt in enemy.enemyOpts:
                if opt.name == enemy_name:
                    enemy = opt
                    break

        shuffled[plando_enemy_key] = (enemy_id, enemy, True)
        del to_shuffle[plando_enemy_key]
    
    enemy_choices = list(enemy_actor_types.keys())
    for enemy_key in to_shuffle:
        enemy_type = to_shuffle[enemy_key]
        if type(enemy_type) is EnemyLocation: # EnemyLocation with type restrictions
            restriction = enemy_type.restrictions
            meets_enemy_restrictions = enemy_type.meets_enemy_restrictions
            disallowed_enemies = enemy_type.disallowed_enemies
            #enemy_type = enemy_type.id
        else: # Just an enemy ID
            #enemy_type = to_shuffle[enemy_key]
            restriction = []
            meets_enemy_restrictions = []
            disallowed_enemies = []
        enemy_choices = list(get_restricted_enemy_types(enemy_actor_types, restriction, meets_enemy_restrictions, disallowed_enemies))
        choice = random.choice(enemy_choices)
        enemy = enemy_actor_types[choice]
        if type(enemy) is EnemyWithOpts:
            enemy = random.choice(enemy.enemyOpts)
        shuffled[enemy_key] = (choice, enemy, True)
    return shuffled

# Get a list of allowed enemy types based on the restrictions passed in
# enemy_actor_types - enemy dict
# restrictions - list of location's restrictions. Enemies must be tagged with the same restriction in order to be included
# meets_enemy_restrictions - list of the enemy restrictions that the location meets. Enemies with ENEMY_RESTRICTIONs will not be allowed unless the location is tagged with that restriction
# Disallowed enemies - list of enemy actor ID's to explicitly exclude from a location
def get_restricted_enemy_types(enemy_actor_types: dict[int,Enemy], restrictions: list[LOCATION_RESTRICTION], meets_enemy_restrictions: list[ENEMY_RESTRICTION], disallowed_enemies: list[int]):
    restricted_enemy_actor_types: dict[int,Enemy] = {}
    for enemy_id in enemy_actor_types:
        enemy = enemy_actor_types[enemy_id]
        meets_restrictions = True
        for restriction in restrictions:
            if restriction not in enemy.categories:
                meets_restrictions = False
                break
        for disallowed in disallowed_enemies:
            if enemy_id == disallowed:
                meets_restrictions = False
                break
        for required_category in enemy.required_categories:
            if required_category not in meets_enemy_restrictions:
                meets_restrictions = False
                break
        if meets_restrictions:
            restricted_enemy_actor_types[enemy_id] = enemy
    return restricted_enemy_actor_types

def patch_enemies(world: World,enemy_list: dict[tuple[int,int,int,int],Actor], shuffled_enemies: dict[tuple[int,int,int,int], tuple[int,Enemy, bool]], rom: Rom, scene_data: list[Scene]):
    
    switch_flags_table = []
    for enemy_key in shuffled_enemies:
        keys = [enemy_key]
        if enemy_key in base_enemy_alts.keys():
            alt = base_enemy_alts[enemy_key]
            if type(alt) is list:
                keys.extend(base_enemy_alts[enemy_key])
            else:
                keys.append(base_enemy_alts[enemy_key])
        for key in keys:
            enemy_id, enemy, shuffled = shuffled_enemies[enemy_key]
            if key in enemy_list.keys():
                enemy_actor = enemy_list[key]
                if shuffled:
                    enemy_actor.rot_x = 0
                    enemy_actor.rot_z = 0
                    enemy_actor.id = enemy_id
                    enemy_actor.var = enemy.var
                    if key in world.enemy_list and type(world.enemy_list[key]) is EnemyLocation:
                        if world.enemy_list[key].patch_func:
                            world.enemy_list[key].patch_func(enemy_actor)
                    rom.write_bytes(enemy_actor.addr, enemy_actor.get_bytes())
                    if key in world.enemy_list and type(world.enemy_list[key]) is EnemyLocation:
                        if world.enemy_list[key].switch_flag >= 0:
                            switch_flags_table.append((key,world.enemy_list[key].switch_flag))
            else:
                print(f"Missing enemy actor {key}")
    
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
    rom.write_bytes(rom.sym('KILL_SWITCH_TABLE'), switch_flags_table_bytes)

# Nabooru knuckle enemizer patch function
# Patch the door to work on room clear instead of switch flag
def patch_nabooru_knuckle(rom: Rom, scene_data: list[Scene]):
    nabooru_transition = scene_data[23].transition_actors[1]
    nabooru_transition.var = 0x40
    rom.write_bytes(nabooru_transition.addr, nabooru_transition.get_bytes())

# Add patch funcs here, we'll call them in a loop in patches.py
enemizer_patches = [
    patch_nabooru_knuckle
]