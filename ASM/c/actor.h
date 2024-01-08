#ifndef ACTOR_H
#define ACTOR_H

#include "z64.h"
#include <stdbool.h>
#include "get_items.h"

typedef enum {
    #include "actor_table.h"
    /* 0x0192 */ ACTOR_ID_MAX // originally "ACTOR_DLF_MAX"
} ActorID;

#define ACTOR_OVERLAY_TABLE_ADDR 0x800E8530

typedef struct {
    /* 0x00 */ int16_t id;
    /* 0x02 */ uint8_t category; // Classifies actor and determines when it will update or draw
    /* 0x04 */ uint32_t flags;
    /* 0x08 */ int16_t objectId;
    /* 0x0C */ uint32_t instanceSize;
    /* 0x10 */ ActorFunc init; // Constructor
    /* 0x14 */ ActorFunc destroy; // Destructor
    /* 0x18 */ ActorFunc update; // Update Function
    /* 0x1C */ ActorFunc draw; // Draw function
} ActorInit; // size = 0x20

typedef struct {
    /* 0x00 */ uintptr_t vromStart;
    /* 0x04 */ uintptr_t vromEnd;
    /* 0x08 */ void* vramStart;
    /* 0x0C */ void* vramEnd;
    /* 0x10 */ void* loadedRamAddr; // original name: "allocp"
    /* 0x14 */ ActorInit* initInfo;
    /* 0x18 */ char* name;
    /* 0x1C */ uint16_t allocType; // See `ACTOROVL_ALLOC_` defines
    /* 0x1E */ int8_t numLoaded; // original name: "clients"
} ActorOverlay; // size = 0x20

// New data added to the end of every actor.
// Make sure the size of this struct is equal to the amount of space added added in Actor_Spawn_Malloc_Hack from actor.asm
typedef struct {
    /* 0x00 */ uint16_t actor_id;
    /* 0x02 */ xflag_t flag;
} ActorAdditionalData;

void Actor_After_UpdateAll_Hack(z64_actor_t *actor, z64_game_t *game);
void Actor_StoreFlagByIndex(z64_actor_t *actor, z64_game_t* game, uint16_t actor_index);
void Actor_StoreFlag(z64_actor_t *actor, z64_game_t* game, xflag_t flag);
void Actor_StoreChestType(z64_actor_t *actor, z64_game_t *game);
z64_actor_t *Actor_SpawnEntry_Hack(void *actorCtx, ActorEntry *actorEntry, z64_game_t *globalCtx);
bool spawn_override_silver_rupee(ActorEntry *actorEntry, z64_game_t *globalCtx, bool* overridden);
void after_spawn_override_silver_rupee(z64_actor_t* actor, bool overridden);
ActorAdditionalData* Actor_GetAdditionalData(z64_actor_t* actor);
bool spawn_override_enemizer(ActorEntry *actorEntry, z64_game_t *globalCtx, bool* overridden);
#endif
