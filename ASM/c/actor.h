#ifndef ACTOR_H
#define ACTOR_H

#include "z64.h"
#include <stdint.h>

void Actor_SetWorldToHome_End(z64_actor_t *actor);
void Actor_After_UpdateAll_Hack(z64_actor_t *actor, z64_game_t* game);
void Actor_StoreFlagInRotation(z64_actor_t* actor, z64_game_t* game, uint16_t actor_index);
void Actor_StoreChestType(z64_actor_t* actor, z64_game_t* game);

typedef struct {
    /* 0x00 */ int16_t id;
    /* 0x02 */ uint8_t category; // Classifies actor and determines when it will update or draw
    /* 0x04 */ uint32_t flags;
    /* 0x08 */ int16_t objectId;
    /* 0x0C */ uint32_t instanceSize;
    /* 0x10 */ void* init; // Constructor
    /* 0x14 */ void* destroy; // Destructor
    /* 0x18 */ void* update; // Update Function
    /* 0x1C */ void* draw; // Draw function
} ActorInit; // size = 0x20

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
