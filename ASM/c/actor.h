#ifndef ACTOR_H
#define ACTOR_H

#include "z64.h"
#include <stdbool.h>
#include <stdint.h>

void Actor_SetWorldToHome_End(z64_actor_t *actor);
void Actor_After_UpdateAll_Hack(z64_actor_t *actor, z64_game_t *game);
void Actor_StoreFlagInRotation(z64_actor_t *actor, z64_game_t *game, uint16_t actor_index);
void Actor_StoreChestType(z64_actor_t *actor, z64_game_t *game);
z64_actor_t *Actor_SpawnEntry_Hack(void *actorCtx, ActorEntry *actorEntry, z64_game_t *globalCtx);
bool spawn_override_silver_rupee(ActorEntry *actorEntry, z64_game_t *globalCtx);

typedef enum {
    /* 0x00 */ ACTORCAT_SWITCH,
    /* 0x01 */ ACTORCAT_BG,
    /* 0x02 */ ACTORCAT_PLAYER,
    /* 0x03 */ ACTORCAT_EXPLOSIVE,
    /* 0x04 */ ACTORCAT_NPC,
    /* 0x05 */ ACTORCAT_ENEMY,
    /* 0x06 */ ACTORCAT_PROP,
    /* 0x07 */ ACTORCAT_ITEMACTION,
    /* 0x08 */ ACTORCAT_MISC,
    /* 0x09 */ ACTORCAT_BOSS,
    /* 0x0A */ ACTORCAT_DOOR,
    /* 0x0B */ ACTORCAT_CHEST
} ActorCategory;

#endif
