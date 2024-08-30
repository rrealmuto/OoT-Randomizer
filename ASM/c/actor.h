#ifndef ACTOR_H
#define ACTOR_H

#include "z64.h"
#include "get_items.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
    #include "actor_table.h"
    /* 0x0192 */ ACTOR_ID_MAX // originally "ACTOR_DLF_MAX"
} ActorID;

// New data added to the end of every actor.
// Make sure the size of this struct is equal to the amount of space added added in Actor_Spawn_Malloc_Hack from actor.asm
typedef struct {
    /* 0x00 */ uint16_t actor_id;
    /* 0x02 */ xflag_t flag;
    /* 0x04 */ uint8_t minimap_draw_flags;
} ActorAdditionalData;

void Actor_After_UpdateAll_Hack(z64_actor_t* actor, z64_game_t* game);
void Actor_StoreFlagByIndex(z64_actor_t* actor, z64_game_t* game, uint16_t actor_index);
void Actor_StoreFlag(z64_actor_t* actor, z64_game_t* game, xflag_t flag);
void Actor_StoreChestType(z64_actor_t* actor, z64_game_t *game);
z64_actor_t *Actor_SpawnEntry_Hack(void* actorCtx, ActorEntry* actorEntry, z64_game_t* globalCtx);
bool spawn_override_silver_rupee(ActorEntry* actorEntry, z64_game_t* globalCtx, bool* overridden);
void after_spawn_override_silver_rupee(z64_actor_t* actor, bool overridden);
void Actor_BuildFlag(z64_actor_t* actor, xflag_t* flag, uint16_t actor_index, uint8_t subflag);
ActorAdditionalData* Actor_GetAdditionalData(z64_actor_t* actor);
void* Actor_ResolveOverlayAddr(z64_actor_t* actor, void* addr);
override_t get_newflag_override(xflag_t* flag);
bool spawn_override_enemizer(ActorEntry *actorEntry, z64_game_t *globalCtx, bool* overridden);
extern ActorOverlay gActorOverlayTable[];
extern void Actor_SetScale(z64_actor_t* actor, float scale);
#endif
