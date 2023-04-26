#include "actor.h"
#include "get_items.h"
#include "z64.h"
#include "pots.h"
#include "item_table.h"
#include "get_items.h"
#include "obj_kibako.h"
#include "obj_kibako2.h"
#include "obj_comb.h"
#include "textures.h"
#include "en_bb.h"
#include "actor.h"
#include "scene.h"

extern uint8_t POTCRATE_TEXTURES_MATCH_CONTENTS;
extern uint16_t CURR_ACTOR_SPAWN_INDEX;
extern uint8_t SHUFFLE_SILVER_RUPEES;
extern int8_t curr_scene_setup;

#define BG_HAKA_TUBO        0x00BB  // Shadow temple spinning pot
#define BG_SPOT18_BASKET    0x015C  // Goron city spinning pot
#define OBJ_COMB            0x19E   // Beehive
#define OBJ_MURE3           0x1AB   // Rupee towers/circles
#define OBJ_TSUBO           0x0111  // Pot
#define EN_ITEM00           0x0015  // Collectible item
#define EN_TUBO_TRAP        0x11D   // Flying Pot
#define OBJ_KIBAKO          0x110   // Small Crate
#define OBJ_KIBAKO2         0x1A0   // Large Crate
#define EN_G_SWITCH         0x0117 //Silver Rupee
#define EN_ANUBICE_TAG      0x00F6  // Anubis Spawner
#define EN_IK               0x0113  // Iron Knuckes
#define EN_SW               0x0095  // Skullwalltula
#define EN_BB               0x0069  // Bubble

ActorOverlay* gActorOverlayTable = (ActorOverlay*)ACTOR_OVERLAY_TABLE_ADDR;

// Get a pointer to the additional data that is stored at the end of every actor
// This is calculated as the actor's address + the actor instance size from the overlay table.
ActorAdditionalData* Actor_GetAdditionalData(z64_actor_t* actor) {
    ActorOverlay overlayEntry = gActorOverlayTable[actor->actor_id]; // Get the overlay table entry
    ActorInit* actorInit = (ActorInit*)((uint32_t)overlayEntry.initInfo - (int32_t)((uint32_t)overlayEntry.vramStart - (uint32_t)overlayEntry.loadedRamAddr)); // Get the init info from the overlay. This is stored in the overlay as a VRAM address so need to convert.

    return (ActorAdditionalData*)(((uint8_t*)actor) + actorInit->instanceSize); // Calculate and return pointer to the new data.
}

// Called at the end of Actor_SetWorldToHome
// Reset the rotations for any actors that we may have passed data in through Actor_Spawn
void Actor_SetWorldToHome_End(z64_actor_t *actor) {
    switch (actor->actor_id) {
        case BG_HAKA_TUBO:
        case BG_SPOT18_BASKET:
        case OBJ_MURE3:
        case OBJ_COMB: {
            actor->rot_world.z = 0;
            break;
        default:
            break;
        }
    }
}

// Called from Actor_UpdateAll when spawning the actors in the scene's/room's actor list.
// For Pots/Crates/Beehives, sets the actors spawn index into unused y/z rotation fields
// This works because this hack occurs after the actor has been spawned and Actor_SetWorldToHome has been called
// Otherwise the actor would be rotated :)
// Now that we resized pots/crates/beehives we could probably just store this info in new space in the actor. But this works for now.
// Prior to being called, CURR_ACTOR_SPAWN_INDEX is set to the current position in the actor spawn list.
void Actor_After_UpdateAll_Hack(z64_actor_t *actor, z64_game_t* game) {
    Actor_StoreFlag(actor, game, CURR_ACTOR_SPAWN_INDEX);
    Actor_StoreChestType(actor, game);

    // Add additional actor hacks here. These get called shortly after the call to actor_init
    // Hacks are responsible for checking that they are the correct actor.
    bb_after_init_hack(actor, game);
    
    CURR_ACTOR_SPAWN_INDEX = 0; // reset CURR_ACTOR_SPAWN_INDEX
}

// For pots/crates/beehives, store the flag in the new space in the actor instance.
// Flag consists of the room #, scene setup, and the actor index
void Actor_StoreFlag(z64_actor_t* actor, z64_game_t* game, uint16_t actor_index) {
    // Zeroize extra data;
    ActorAdditionalData* extra = Actor_GetAdditionalData(actor);
    extra->actor_id = 0;
    extra->flag = 0;

    uint16_t flag = (actor_index) | (actor->room_index << 8) | curr_scene_setup << 14; // Calculate the flag
    flag = resolve_alternative_flag(game->scene_index, flag);
    extra->actor_id = actor_index;
    if(actor->actor_type == ACTORCAT_ENEMY && actor->actor_id != 0x0197) //Hack for most enemies. Specifically exclude gerudo fighters (0x197)
    {
        extra->flag = flag;
        return;
    }
    
    switch(actor->actor_id)
    {
        // For the following actors we store the flag in the z rotation
        case OBJ_TSUBO:
        case EN_TUBO_TRAP:
        case OBJ_KIBAKO:
        case OBJ_COMB: 
        case OBJ_KIBAKO2:
        case EN_IK: // Check for iron knuckles (they use actor category 9 (boss) and change to category 5 but a frame later if the object isnt loaded)
        case EN_SW: // Check for skullwalltula (en_sw). They start as category 4 (npc) and change to category 5 but a frame later if the object isnt laoded
        case EN_ANUBICE_TAG: //Check for anubis spawns
        {
            extra->flag = flag;
            break;
        }
        default: {
            break;
        }
    }
}

// Get an override for an actor with the new flags. If the override doesn't exist, or flag has already been set, return 0.
override_t get_newflag_override(z64_actor_t *actor, z64_game_t *game) {
    override_t override = lookup_override(actor, game->scene_index, 0);
    if(override.key.all != 0)
    {
        if(!Get_NewOverrideFlag(Actor_GetAdditionalData(actor)->flag))
        {
            return override;
        }
    }
    return (override_t) { 0 };
}

// For pots/crates/beehives match contents, determine the override and store the chest type in new space in the actor instance
// So we don't have to hit the override table every frame.
void Actor_StoreChestType(z64_actor_t* actor, z64_game_t* game) {
    uint8_t* pChestType = NULL;
    override_t override = { 0 };

    if (actor->actor_id == OBJ_TSUBO) { // Pots
        override = get_newflag_override(actor, game);
        pChestType = &(((ObjTsubo *)actor)->chest_type);
    } else if (actor->actor_id == EN_TUBO_TRAP) { // Flying Pots
        override = get_newflag_override(actor, game);
        pChestType = &(((EnTuboTrap *)actor)->chest_type);
    } else if (actor->actor_id == OBJ_KIBAKO2) { // Large Crates
        override = get_newflag_override(actor, game);
        pChestType = &(((ObjKibako2 *)actor)->chest_type);
    } else if (actor->actor_id == OBJ_KIBAKO) { // Small wooden crates
        override = get_newflag_override(actor, game);
        pChestType = &(((ObjKibako *)actor)->chest_type);
    } else if (actor->actor_id == OBJ_COMB) {
        override = get_beehive_override(actor, game);
        pChestType = &(((ObjComb *)actor)->chest_type);
    }
    if (override.key.all != 0 && pChestType != NULL) { // If we don't have an override key, then either this item doesn't have an override entry, or it has already been collected.
        if (POTCRATE_TEXTURES_MATCH_CONTENTS == PTMC_UNCHECKED && override.key.all > 0) { // For "unchecked" PTMC setting: Check if we have an override which means it wasn't collected.
            *pChestType = GILDED_CHEST;
        } else if(POTCRATE_TEXTURES_MATCH_CONTENTS == PTMC_CONTENTS) {
            uint16_t item_id = resolve_upgrades(override.value.base.item_id);
            item_row_t *row = get_item_row(override.value.looks_like_item_id);
            if (row == NULL) {
                row = get_item_row(override.value.base.item_id);
            }
            *pChestType = row->chest_type;
        } else {
            *pChestType = 0;
        }
    }
}

typedef void(*actor_after_spawn_func)(z64_actor_t* actor, bool overridden);

z64_actor_t *Actor_SpawnEntry_Hack(void *actorCtx, ActorEntry *actorEntry, z64_game_t *globalCtx) {
    bool continue_spawn = true;
    bool overridden = false;
    actor_after_spawn_func after_spawn_func = NULL;
    switch (actorEntry->id) {
        case EN_G_SWITCH: {
            continue_spawn = spawn_override_silver_rupee(actorEntry, globalCtx, &overridden);
            after_spawn_func = after_spawn_override_silver_rupee;
            break;
        }
        default: {
            break;
        }
    }
    z64_actor_t *spawned = NULL;
    if (continue_spawn) {
        spawned = z64_SpawnActor(actorCtx, globalCtx, actorEntry->id, actorEntry->pos.x, actorEntry->pos.y, actorEntry->pos.z,
            actorEntry->rot.x, actorEntry->rot.y, actorEntry->rot.z, actorEntry->params);
        if(spawned && after_spawn_func) {
            after_spawn_func(spawned, overridden);
        }
    }
    return spawned;
}

// Override silver rupee spawns.
bool spawn_override_silver_rupee(ActorEntry *actorEntry, z64_game_t *globalCtx, bool* overridden) {
    *overridden = false;
    if (SHUFFLE_SILVER_RUPEES) { // Check if silver rupee shuffle is enabled.
        uint16_t flag = (curr_scene_setup << 14) | (globalCtx->room_index << 8) | CURR_ACTOR_SPAWN_INDEX;
        flag = resolve_alternative_flag(globalCtx->scene_index, flag);
        uint8_t type = (actorEntry->params >> 0x0C) & 0xF;
        if (type != 1) { // only override actual silver rupees, not the switches or pots.
            return true;
        }
        override_t override = lookup_override_by_newflag(flag, globalCtx->scene_index);
        if (override.key.all != 0) {
            if (type == 1 && !Get_NewOverrideFlag(flag)) {
                // Spawn a green rupee which will be overridden using the collectible hacks.
                actorEntry->params = 0;
                actorEntry->id = EN_ITEM00;
                *overridden = true;
                return true;
            }
        }
        return false;
    }
    return true;
}

// After silver rupee spawns as enitem00
void after_spawn_override_silver_rupee(z64_actor_t* spawned, bool overridden)
{
    if(overridden) {
        EnItem00* this = (EnItem00*)spawned;
        this->is_silver_rupee = true;
        this->collider.info.bumper.dmgFlags = 0; //Remove clear the bumper collider flags so it doesn't interact w/ boomerang
    }
}

//Return 1 to not spawn the actor, 0 to spawn the actor
//If enemy drops setting is enabled, check if the flag for this actor hasn't been set and make sure to spawn it.
//Flag is the index of the actor in the actor spawn list, or -1 if this function is not being called at the room init.
//Parent will be set if called by Actor_SpawnAsChild
uint8_t Actor_Spawn_Clear_Check_Hack(z64_game_t* globalCtx, ActorInit* actorInit, uint16_t flag, z64_actor_t* parent)
{
    //probably need to do something specific for anubis spawns because they use the spawner items. Maybe flare dancers too?
    if(actorInit->id == 0x00E0 && parent != NULL)
    {
        flag = Actor_GetAdditionalData(parent)->flag;
    }
    if((actorInit->category == ACTORCAT_ENEMY) && z64_Flags_GetClear(globalCtx, globalCtx->room_index))
    {
        //Check if this we're spawning an actor from the room's actor spawn list
        if(flag > 0)
        {
            flag |= globalCtx->room_index << 8;
            //Build a dummy override
            override_t override = lookup_override_by_newflag(flag, globalCtx->scene_index);
            //Check if this actor is in the override list
            if(override.key.all != 0 && !(Get_NewOverrideFlag(flag)>0))
            {
                return 0;
            }
            return 1;
        }
        
        return 1;
    }
    

    return 0;
}
