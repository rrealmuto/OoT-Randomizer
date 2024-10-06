#ifndef ACTOR_H
#define ACTOR_H

#include "z64.h"
#include "get_items.h"
#include <stdbool.h>
#include <stdint.h>

//
#define ACTOR_FLAG_0 (1 << 0)

//
#define ACTOR_FLAG_2 (1 << 2)

//
#define ACTOR_FLAG_3 (1 << 3)

//
#define ACTOR_FLAG_4 (1 << 4)

//
#define ACTOR_FLAG_5 (1 << 5)

//
#define ACTOR_FLAG_6 (1 << 6)

// hidden or revealed by Lens of Truth (depending on room lensMode)
#define ACTOR_FLAG_REACT_TO_LENS (1 << 7)

// Signals that player has accepted an offer to talk to an actor
// Player will retain this flag until the player is finished talking
// Actor will retain this flag until `Actor_TalkOfferAccepted` is called or manually turned off by the actor
#define ACTOR_FLAG_TALK (1 << 8)

//
#define ACTOR_FLAG_9 (1 << 9)

//
#define ACTOR_FLAG_10 (1 << 10)

//
#define ACTOR_FLAG_ENKUSA_CUT (1 << 11)

// Actor will not shake when a quake occurs
#define ACTOR_FLAG_IGNORE_QUAKE (1 << 12)

//
#define ACTOR_FLAG_13 (1 << 13)

//
#define ACTOR_FLAG_14 (1 << 14)

//
#define ACTOR_FLAG_15 (1 << 15)

//
#define ACTOR_FLAG_16 (1 << 16)

//
#define ACTOR_FLAG_17 (1 << 17)

//
#define ACTOR_FLAG_18 (1 << 18)

//
#define ACTOR_FLAG_19 (1 << 19)

//
#define ACTOR_FLAG_20 (1 << 20)

//
#define ACTOR_FLAG_21 (1 << 21)

// ignores point lights but not directional lights (such as environment lights)
#define ACTOR_FLAG_IGNORE_POINT_LIGHTS (1 << 22)

//
#define ACTOR_FLAG_23 (1 << 23)

//
#define ACTOR_FLAG_24 (1 << 24)

//
#define ACTOR_FLAG_25 (1 << 25)

//
#define ACTOR_FLAG_26 (1 << 26)

//
#define ACTOR_FLAG_27 (1 << 27)

//
#define ACTOR_FLAG_28 (1 << 28)

#define UPDBGCHECKINFO_FLAG_0 (1 << 0) // check wall
#define UPDBGCHECKINFO_FLAG_1 (1 << 1) // check ceiling
#define UPDBGCHECKINFO_FLAG_2 (1 << 2) // check floor and water
#define UPDBGCHECKINFO_FLAG_3 (1 << 3)
#define UPDBGCHECKINFO_FLAG_4 (1 << 4)
#define UPDBGCHECKINFO_FLAG_5 (1 << 5) // unused
#define UPDBGCHECKINFO_FLAG_6 (1 << 6) // disable water ripples
#define UPDBGCHECKINFO_FLAG_7 (1 << 7) // alternate wall check?

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

typedef struct enemy_list_entry_t {
    int16_t id;
    uint16_t var;
} enemy_list_entry_t;

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
#endif
