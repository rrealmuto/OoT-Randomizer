#ifndef ENEMY_SPAWN_SHUFFLE_H
#define ENEMY_SPAWN_SHUFFLE_H

#include <stdbool.h>
#include "z64.h"

#define NUM_ENEMY_SOULS 47

#define SOUL_ID_STALFOS 0
#define SOUL_ID_OCTOROK 1
#define SOUL_ID_WALLMASTER 2
#define SOUL_ID_DODONGO 3
#define SOUL_ID_KEESE 4
#define SOUL_ID_TEKTITE 5
#define SOUL_ID_PEAHAT 6
#define SOUL_ID_LIZALFOS_AND_DINALFOS 7
#define SOUL_ID_GOHMA_LARVAE 8
#define SOUL_ID_SHABOM 9
#define SOUL_ID_BABY_DODONGO 10
#define SOUL_ID_BIRI_AND_BARI 11
#define SOUL_ID_TAILPASARAN 12
#define SOUL_ID_SKULLTULA 13
#define SOUL_ID_TORCH_SLUG 14
#define SOUL_ID_MOBLIN 15
#define SOUL_ID_ARMOS 16
#define SOUL_ID_DEKU_BABA 17
#define SOUL_ID_DEKU_SCRUB 18
#define SOUL_ID_BUBBLE 19
#define SOUL_ID_BEAMOS 20
#define SOUL_ID_FLOORMASTER 21
#define SOUL_ID_REDEAD_AND_GIBDO 22
#define SOUL_ID_SKULLWALLTULA 23
#define SOUL_ID_FLARE_DANCER 24
#define SOUL_ID_DEAD_HAND 25
#define SOUL_ID_SHELL_BLADE 26
#define SOUL_ID_LIKE_LIKE 27
#define SOUL_ID_SPIKE_ENEMY 28
#define SOUL_ID_ANUBIS 29
#define SOUL_ID_IRON_KNUCKLE 30
#define SOUL_ID_SKULL_KID 31
#define SOUL_ID_FLYING_POT 32
#define SOUL_ID_FREEZARD 33
#define SOUL_ID_STINGER 34
#define SOUL_ID_WOLFOS 35
#define SOUL_ID_GUAY 36
#define SOUL_ID_QUEEN_GOHMA 37
#define SOUL_ID_KING_DODONGO 38
#define SOUL_ID_BARINADE 39
#define SOUL_ID_PHANTOM_GANON 40
#define SOUL_ID_VOLVAGIA 41
#define SOUL_ID_MORPHA 42
#define SOUL_ID_BONGO_BONGO 43
#define SOUL_ID_TWINROVA 44
#define SOUL_ID_JABU_JABU_TENTACLE 45
#define SOUL_ID_DARK_LINK 46
#define SOUL_ID_GANONDORF 47

// Enemy spawn shuffle spawn check override function.
// Return true if the actor entry should be handled by enemy spawn shuffle
typedef bool (*alt_spawn_override_fn)(ActorEntry *actorEntry, z64_game_t *globalCtx);

bool spawn_check_gs(ActorEntry *actorEntry, z64_game_t *globalCtx);
bool spawn_check_big_octo(ActorEntry *actorEntry, z64_game_t *globalCtx);
bool spawn_check_armos(ActorEntry *actorEntry, z64_game_t *globalCtx);
bool spawn_check_skullkid(ActorEntry *actorEntry, z64_game_t *globalCtx);
bool flags_getsoul(int table_index);
bool flags_setsoul(int table_index);
bool get_soul_enabled(int table_index);
bool toggle_soul_enabled(int table_index);

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
extern char* SOUL_MENU_NAMES[];

#define ENEMY_SPAWN_TABLE_ENTRY(actor_id_,index_,flags_,override_func_) {.actor_id = actor_id_, .index = index_, .flags = flags_, .override_func = override_func_}

bool spawn_override_enemy_spawn_shuffle(ActorEntry *actorEntry, z64_game_t *globalCtx, SPAWN_FLAGS flag);

#endif