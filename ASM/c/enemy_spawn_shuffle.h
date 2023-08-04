#ifndef ENEMY_SPAWN_SHUFFLE_H
#define ENEMY_SPAWN_SHUFFLE_H

#include <stdbool.h>
#include "z64.h"

// Enemy spawn shuffle spawn check override function.
// Return true if the actor entry should be handled by enemy spawn shuffle
typedef bool (*alt_spawn_override_fn)(ActorEntry *actorEntry, z64_game_t *globalCtx);

bool spawn_check_gs(ActorEntry *actorEntry, z64_game_t *globalCtx);
bool spawn_check_big_octo(ActorEntry *actorEntry, z64_game_t *globalCtx);
bool spawn_check_armos(ActorEntry *actorEntry, z64_game_t *globalCtx);
bool spawn_check_skullkid(ActorEntry *actorEntry, z64_game_t *globalCtx);
bool flags_getsoul(int table_index);

typedef enum SPAWN_FLAGS {
    SPAWN_FLAGS_SPAWNENTRY =  1,
    SPAWN_FLAGS_ACTORSPAWN = 2
} SPAWN_FLAGS;

typedef struct enemy_spawn_table_entry {
    uint16_t actor_id;
    uint8_t index;
    SPAWN_FLAGS flags;
    alt_spawn_override_fn override_func;
} enemy_spawn_table_entry;

extern uint8_t CFG_ENEMY_SPAWN_SHUFFLE;

#define ENEMY_SPAWN_TABLE_ENTRY(actor_id_,index_,flags_,override_func_) {.actor_id = actor_id_, .index = index_, .flags = flags_, .override_func = override_func_}

bool spawn_override_enemy_spawn_shuffle(ActorEntry *actorEntry, z64_game_t *globalCtx, SPAWN_FLAGS flag);

#endif