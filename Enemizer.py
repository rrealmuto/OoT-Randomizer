import random
from ProcessActors import *

class Enemy:
    def __init__(self, name: str, var: int = 0, filter_func = None):
        self.name = name
        self.var = var
        self.filter_func = filter_func


# Filter functions, return false to filter out enemy from being shuffled

def filter_skullwalltula(actor: Actor):
    # Filter gold skulltulas, type == 4 or type == 5
    type = (actor.var & 0xE000) >> 13
    return not (type == 4 or type == 5)

def filter_armos(actor: Actor):
    # Filter armos, var == 0 is a pushable statue so we don't want to filter these
    return actor.var != 0

def filter_skullkids(actor: Actor):
    # Filter skull kids, type <= 6 are the ocarina game ones
    type = (actor.var >> 0x0A) & 0x3F
    return type > 6

enemy_actor_types = {
    0x0002: Enemy("Stalfos", var=0x0003),
    0x000E: Enemy("Octorok"),
    0x0011: Enemy("Wallmaster"),
    0x0012: Enemy("Dodongo"),
    0x0013: Enemy("Keese"),
    0x001B: Enemy("Tektite"),
    0x001D: Enemy("Peahat"),
    0x0025: Enemy("Lizalfos/Dinalfos", var=0xFF80),
    0x002B: Enemy("Gohma Larva", var=0x0006),
    0x002D: Enemy("Shabom"),
    0x002F: Enemy("Baby Dodongo"),
    0x0034: Enemy("Biri"),
    0x0063: Enemy("Bari"),
    0x0035: Enemy("Tailpasaran", var=0xFFFF),
    0x0037: Enemy("Skulltula"),
    0x0038: Enemy("Torch Slug"),
    0x004B: Enemy("Moblin"),
    0x0054: Enemy("Armos", filter_func=filter_armos),
    0x0055: Enemy("Deku Baba"),
    0x00C7: Enemy("Whithered Deku Baba"),
    0x0060: Enemy("Deku Scrub"),
    0x0069: Enemy("Bubble", var=0xFFFF),
    0x008A: Enemy("Beamos"),
    0x008E: Enemy("Floormaster"),
    0x0090: Enemy("Redead/Gibdo"),
    0x0095: Enemy("Skullwaltula", filter_func=filter_skullwalltula),
    0x0099: Enemy("Flare Dancer"),
    # 0x00A4: Enemy("Dead Hand"),
    0x00C5: Enemy("Shell Blade"),
    0x00DD: Enemy("Like like"),
    0x00EC: Enemy("Spike"),
    0x00F6: Enemy("Anubis Spawner"),
    0x0113: Enemy("Iron Knuckle", var=0xFF02),
    0x0115: Enemy("Skull Kid", var=0xFFFF, filter_func=filter_skullkids),
    0x0121: Enemy("Freezard"),
    0x018C: Enemy("Stinger"),
    0x003A: Enemy("Singray", var=0x000A),
    0x01AF: Enemy("Wolfos", var=0xFF00),
    0x01C0: Enemy("Guay"),
}


def shuffle_enemies(scenes: list[Scene], rom: Rom):
    enemy_list: dict[tuple[int,int,int,int],Actor] = {}
    for scene in scenes:
        for room in scene.rooms:
            for setup in room.setups:
                actors = room.setups[setup].actors
                for i in range(0, len(actors)):
                    if actors[i].id in enemy_actor_types.keys():
                        enemy_list[(scene.id, room.id,setup,i)] = (actors[i])
    
    enemy_choices = list(enemy_actor_types.keys())
    for enemy_id in enemy_list:
        enemy = enemy_list[enemy_id]
        choice = random.choice(enemy_choices)
        #choice = 0x002B
        print(f"{enemy_id} - {enemy_actor_types[choice].name} - Var={hex(enemy_actor_types[choice].var)}")
        filter = enemy_actor_types[enemy.id].filter_func
        if(filter is None or filter(enemy)):
            if(choice != enemy_id):
                enemy.rot_x = 0
                enemy.rot_z = 0
                enemy.id = choice
                enemy.var = enemy_actor_types[choice].var
                rom.write_bytes(enemy.addr, enemy.get_bytes())
            else:
                print("Skipping duplicate enemy")
        