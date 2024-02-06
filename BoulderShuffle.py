from Boulders import *
from Search import Search
from World import World

def set_boulders(worlds: list[World]):
    for world in worlds:
        build_boulders(world)

def build_boulders(world):
    # Build the full boulder list from the overworld ones + dungeons
    boulders = boulder_list.copy()

    mq_dungeons = [dungeon for dungeon in world.dungeon_mq if world.dungeon_mq[dungeon] == True]
    vanilla_dungeons = [dungeon for dungeon in world.dungeon_mq if world.dungeon_mq[dungeon] == False]

    for dungeon in mq_dungeons:
        if dungeon in mq_dungeon_boulders.keys():
            boulders.update(mq_dungeon_boulders[dungeon])
    
    for dungeon in vanilla_dungeons:
        if dungeon in vanilla_dungeon_boulders.keys():
            boulders.update(vanilla_dungeon_boulders[dungeon])
    
    world.boulder_list = boulders
    shuffled_boulders_by_id = {}
    shuffled_boulders = {}
    boulder_keys = list(boulders)
    target_types = [boulders[boulder][list(boulders[boulder])[0]]['type'] for boulder in boulders]
    for i in range(0,len(boulders)):
        boulder_key = boulder_keys[i]
        boulder = boulders[boulder_key] # Contains a dict of boulders for this because we have multiple scene setups
        shuffled_boulders [boulder_key] = target_types[i]

        for id in boulder:
            if 'patch' in list(boulder[id]):
                patch_func = boulder[id]['patch']
            else:
                patch_func = None
            shuffled_boulders_by_id[(id[0], id[1], id[2], id[3])] = (boulder_keys[i], target_types[i], patch_func)
    world.boulders = shuffled_boulders
    world.boulders_by_id = shuffled_boulders_by_id

def shuffle_boulders(worlds):
    complete_itempool = [item for world in worlds for item in world.get_itempool_with_dungeon_items()]
    non_drop_locations = [location for world in worlds for location in world.get_locations() if location.type not in ('Drop', 'Event')]

    for world in worlds:
        unreached = True
        while unreached:
            orig_boulders = world.boulders.copy()
            orig_boulders_by_id = world.boulders_by_id.copy()
            max_search = Search.max_explore([world.state], complete_itempool)
            max_search.visit_locations(non_drop_locations)
            locations_to_ensure_reachable = list(filter(max_search.visited, non_drop_locations))

            world.boulders, world.boulders_by_id = _shuffle_boulders(world)
            max_search = Search.max_explore([world.state], complete_itempool)
            max_search.visit_locations(non_drop_locations)
            reachable_locations = list(filter(max_search.visited, non_drop_locations))

            unreached = False
            for location in locations_to_ensure_reachable:
                if location not in reachable_locations:
                    unreached = True
                    print(location.name)
            if unreached:
                world.boulders = orig_boulders
                world.boulders_by_id = orig_boulders_by_id
                print("Retrying...")


def _shuffle_boulders(world) -> tuple[dict[str, dict[tuple[int,int,int,int], dict[str, any]]], dict[tuple[int,int,int,int], tuple[str,BOULDER_TYPE]]]:
    # Build the full boulder list from the overworld ones + dungeons
    boulders = world.boulders

    # Get each boulder's type which we'll shuffle
    target_types = [boulders[boulder] for boulder in boulders]
    if world.settings.shuffle_boulders:
        random.shuffle(target_types)

    shuffled_boulders_by_id = {}
    shuffled_boulders = {}
    boulder_keys = list(boulders)

    # Place plandoed boulders first
    if world.distribution.boulders:
        for boulder_id in world.distribution.boulders.keys():
            boulder_keys.remove(boulder_id)
            shuffled_boulders[boulder_id] = world.distribution.boulders[boulder_id]

    # Place priority boulders if all locations reachable
    if world.settings.reachable_locations == 'all':
        for priority_boulder_id in priority_boulders.keys():
            priority_type = random.choice(priority_boulders[priority_boulder_id])
            target_types.remove(priority_type)
            boulder_keys.remove(priority_boulder_id)
            shuffled_boulders[priority_boulder_id] = priority_type
             
    for boulder_id in boulder_keys:
        shuffled_boulders [boulder_id] = target_types.pop(0)
        #shuffled_boulders[boulder_key] = BOULDER_TYPE.BROWN
        
    for boulder_id in shuffled_boulders:
        boulder = world.boulder_list[boulder_id]
        for flag in boulder:
            if 'patch' in list(boulder[flag]):
                patch_func = boulder[flag]['patch']
            else:
                patch_func = None
            if 'collider_override' in list(boulder[flag]):
                collider_override = boulder[flag]['collider_override']
            else:
                collider_override = None
            shuffled_boulders_by_id[flag] = (boulder_id, shuffled_boulders[boulder_id], patch_func, collider_override)

    return shuffled_boulders, shuffled_boulders_by_id
