#include "fairy.h"
#include "z64.h"
#include "z64_math.h"
#include "models.h"
#include "get_items.h"
#include "item_table.h"
#include "actor.h"
#include "scene.h"
#include "util.h"
#include "audio.h"

extern uint16_t CURR_ACTOR_SPAWN_INDEX;
extern xflag_t* spawn_actor_with_flag;
extern uint8_t PLAYER_ID;
extern EnElfActionFunc OVL_EnElf_SpinAction;

// Replaces EnElf_Draw call to SkelAnime_Draw
Gfx* EnElf_SkelAnime_Draw_Hack(z64_game_t* play, void** skeleton, z64_xyz_t* jointTable, OverrideLimbDraw overrideLimbDraw, PostLimbDraw postLimbDraw, void* arg, Gfx* gfx) {
    // Check if we have an override
    // arg contains actor pointer
    EnElf* this = (EnElf*)arg;
    if (this->override.key.all) {
        // Update the model if we haven't collected yet
        if(!(this->fairyFlags & FAIRY_FLAG_OVERRIDE_COLLECTED)) {
            lookup_model_by_override(&this->model, this->override);
        }
        // Draw override model
        draw_model(this->model, &this->actor, play, 40.0);
        return play->common.gfx->poly_xlu.p; // Vanilla function assigns POLY_XLU_DISP to the return value of SkelAnime_Draw so we need to actually return it in case we change it
    }

    // Call original SkelAnime_Draw
    return SkelAnime_Draw(play, skeleton, jointTable, overrideLimbDraw, postLimbDraw, arg, gfx);
}

// Normally, rotX, rotY, rotZ are int16_t. However, they are stored on the stack using int32_t so it is ok to do this I think
z64_actor_t* ElElf_FairySpawn_ActorSpawn_Hack(void* actorCtx, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int32_t rotX, int32_t rotY, int32_t rotZ, int16_t params) {
    // We stored the actor instance and current loop index in 2 of the rot variables.
    // They were stored on the stack as 4 bytes but here they are only 2 so how do we get the full 4?

    // Actor should be at 0x1C(sp) // which is this?
    // Loop Variable at 0x18(sp) // which is this?

    EnElf* this = ((EnElf*)(rotY));
    int loopVar = (int)rotX;
    
    xflag_t flag = { 0 };
    bool was_spawned_with_flag = false;
    if(spawn_actor_with_flag) { // Handle the case where the fairy spawn was spawned by something else like the colossus oasis
        was_spawned_with_flag = true;
        spawn_actor_with_flag->subflag = loopVar + 1;
        flag = *spawn_actor_with_flag;
    }
    else {
        Actor_BuildFlag(&this->actor, &flag, CURR_ACTOR_SPAWN_INDEX, loopVar + 1);
    }
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    flag = resolve_alternative_flag(&flag);
    if (!was_spawned_with_flag)
        spawn_actor_with_flag = &flag;
    z64_actor_t* spawned = z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
    if(spawned) {
        EnElf* elf = (EnElf*)spawned;
        elf->override = get_newflag_override(&flag);
    }
    if (!was_spawned_with_flag) // Don't clear the spawn_actor_with flag if this spawner was spawned with it
        spawn_actor_with_flag = NULL;
    return spawned;
}

// Hooked call to Actor_OfferGetItem in main healing fairy action. Actor_OfferGetItem is what normally
// Allows the fairy to be captured in a bottle
int32_t EnElf_Actor_OfferGetItem_Hack(z64_actor_t* actor, z64_game_t* play, int32_t getItemId, float xzRange, float yRange) {
    EnElf* this = (EnElf*)actor;
    if(!this->override.key.all)
        return Actor_OfferGetItem(actor, play, getItemId, xzRange, yRange);
    return 0;
}

// Hooked call to EnElf_SetupAction in the main healing fairy action function for when player interacts w/ the fairy
void EnElf_HealingFairyFlyAndWait_SetupAction_Hook(EnElf* this, EnElfActionFunc actionFunc) {
    // Check if we have an override
    if(this->override.key.all) {
        if (!collectible_mutex) {
            collectible_mutex = this;
            ActorAdditionalData* extras = Actor_GetAdditionalData(&this->actor);
            uint8_t player = this->override.value.base.player;
            uint16_t resolved_item_id = resolve_upgrades(this->override);
            item_row_t* item_row = get_item_row(resolved_item_id);
            
            // Set something in fairyFlags so it knows that it has been collected
            this->fairyFlags |= FAIRY_FLAG_OVERRIDE_COLLECTED;
            Set_NewFlag(&(extras->flag));
            // Make sure model is set. Fairy collected on the same frame that it spawned will break otherwise
            lookup_model_by_override(&this->model, this->override);
            // Check if it's a collectible or major item
            if(item_row->collectible >= 0) {
                SFX_PLAY_CENTERED(GET_ITEM_SEQ_ID);
                dispatch_item(resolved_item_id, this->override.value.base.player, &(this->override), item_row);
                EnElfActionFunc EnElf_SpinAction = resolve_overlay_addr(&OVL_EnElf_SpinAction, this->actor.actor_id);
                this->actionFunc = EnElf_SpinAction;
                collectible_mutex = NULL;
                return;
            }
            
            z64_DisplayTextbox(&z64_game, resolve_item_text_id(item_row, player != PLAYER_ID), NULL);
            dispatch_item(resolved_item_id, this->override.value.base.player, &(this->override), item_row);
            z64_game.msgContext.choiceIndex = 0;
            // New setup function to wait for message box to close

            z64_Audio_PlayFanFare(NA_BGM_SMALL_ITEM_GET);
            this->actionFunc = EnElf_WaitForMessageBox;
        }
    }
    else {
        // No override so just call the original setup function
        //EnElf_SetupAction(this, func_80A03610);
        // NTSC 1.0 - 808874e0
        // Do the stuff that we nop'd
        Health_ChangeBy(&z64_game, 128);
        if (this->fairyFlags & FAIRY_FLAG_BIG) {
            Magic_Fill(&z64_game);
        }

        EnElfActionFunc EnElf_SpinAction = resolve_overlay_addr(&OVL_EnElf_SpinAction, this->actor.actor_id);
        this->actionFunc = EnElf_SpinAction;
    }
}

void EnElf_WaitForMessageBox(EnElf* this, z64_game_t* play) {
    // Check message state:
    if (Message_GetState(&z64_game.msgContext) == 0) {
        reset_collectible_mutex(); // release the mutex
        // Kill the actor
        EnElfActionFunc EnElf_SpinAction = resolve_overlay_addr(&OVL_EnElf_SpinAction, this->actor.actor_id);
        this->actionFunc = EnElf_SpinAction;
    }
    else {
        z64_link.common.frozen = 10;
    }
}

// Hacked call to Actor_Spawn for butterflies when they turn into fairy
z64_actor_t* EnButte_SpawnFairy_Hack(void* actorCtx, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int32_t rotX, int16_t rotY, int32_t rotZ, int16_t params) {
    z64_actor_t* this = (z64_actor_t*)rotX; // We passed actor pointer in rotX
    rotX = 0;

    // Check if we have a flag and that we haven't already collected it
    ActorAdditionalData* extras = Actor_GetAdditionalData(this);
    if(extras->flag.all && !Get_NewFlag(&extras->flag)) {
        // Spawn fairy w/ override
        xflag_t flag = resolve_alternative_flag(&extras->flag);
        spawn_actor_with_flag = &flag;
        z64_actor_t* spawned = z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, 6); // Use param 6 so it doesn't time out
        spawn_actor_with_flag = NULL;
        if(spawned) {
            EnElf* elf = (EnElf*)spawned;
            elf->override = get_newflag_override(&flag);
        }
        return spawned;
    }
    else {
        // Otherwise just spawn it regular
        return z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
    }
}

// Hacked call to Actor_Spawn in ObjMure which is the spawner item for butterflies
// We hacked the spawn params to pass in the ObjMure pointer and the loop variable in rotX and rotY
z64_actor_t* ObjMure_SpawnActors1_Actor_Spawn_Hack(void* actorCtx, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int32_t rotX, int32_t rotY, int16_t rotZ, int16_t params) {
    ObjMure* this = (ObjMure*)rotX;
    int32_t loopVar = rotY;
    // Reset rotX and rotY
    int16_t rotX_real = this->actor.rot_world.x;
    int16_t rotY_real = this->actor.rot_world.y;
    
    ActorAdditionalData* extras = Actor_GetAdditionalData(&this->actor);

    // Build flag
    xflag_t flag = { 0 };
    Actor_BuildFlag(&this->actor, &flag, extras->actor_id, loopVar + 1);
    spawn_actor_with_flag = &flag;
    // Check if we're spawning a butterfly
    if(this->type == 4) {
        // Force butterfly to follow if we haven't collected the fairy
        if (!Get_NewFlag(&flag))
            params = 1;
    }
    z64_actor_t* spawned = z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX_real, rotY_real, rotZ, params);
    spawn_actor_with_flag = NULL;
    return spawned;
}

typedef void (*EnButte_SetupFunc)(z64_actor_t*);
extern EnButte_SetupFunc OVL_EnButte_SetupFollowLink;
extern EnButte_SetupFunc OVL_EnButte_SetupTransformIntoFairy;


// Hacked call to EnButte_SetupFollowLink so we can make it easier to trigger the fairy
void EnButte_SetupFollowLink_Hack(z64_actor_t* this) {
    
    ActorAdditionalData* extras = Actor_GetAdditionalData(this);
    if(!Get_NewFlag(&extras->flag)) {
        // Just trigger the fairy instead of making it follow
        // Make the check a little closer
        if(this->xzdist_from_link < 30.0f) {
            EnButte_SetupFunc EnButte_SetupTransformIntoFairy = resolve_overlay_addr(&OVL_EnButte_SetupTransformIntoFairy, this->actor_id);
            EnButte_SetupTransformIntoFairy(this);
        }
        return;
    }
    EnButte_SetupFunc EnButte_SetupFollowLink = resolve_overlay_addr(&OVL_EnButte_SetupFollowLink, this->actor_id);
    EnButte_SetupFollowLink(this);
}

// Hacked call to ActorSpawn for the oasis in collosus so we can spawn the fairy spawner with an xflag
// We stored actor pointer in rotX, is normally just 0
z64_actor_t* BgSpot11Oasis_SpawnFairies_ActorSpawn_Hack(void* actorCtx, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int32_t rotX, int16_t rotY, int16_t rotZ, int16_t params) {
    // Build flag
    z64_actor_t* this = (z64_actor_t*)rotX;
    ActorAdditionalData* extras = Actor_GetAdditionalData(this);
    xflag_t flag = { 0 };
    Actor_BuildFlag(this, &flag, extras->actor_id, 0);
    spawn_actor_with_flag = &flag;
    z64_actor_t* spawned = z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, 0, rotY, rotZ, params);
    spawn_actor_with_flag = NULL;
    return spawned;
}


z64_actor_t* ShotSun_SpawnFairy_ActorSpawn_Hack(void* actorCtx, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int32_t rotX, int16_t rotY, int16_t rotZ, int16_t params) {
    // Build flag
    z64_actor_t* this = (z64_actor_t*)rotX;
    ActorAdditionalData* extras = Actor_GetAdditionalData(this);
    xflag_t flag = { 0 };
    Actor_BuildFlag(this, &flag, extras->actor_id, 0);
    flag = resolve_alternative_flag(&flag);
    
    // Check if we already have this flag
    if(!Get_NewFlag(&flag)) {
        override_t override = get_newflag_override(&flag);
        spawn_actor_with_flag = &flag;
        z64_actor_t* spawned = z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, 0, rotY, rotZ, 6); // Force it to spawn as a healing fairy that doesn't time out
        if(spawned) {
            ((EnElf*)spawned)->override = override;
        }
        spawn_actor_with_flag = NULL;
        return spawned;
    }
    return z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, 0, rotY, rotZ, params);
}



// Replaces call to Item_DropCollectible in Obj_Bean when spawning fairies after playing song of storms
// Passes loop variable into num param
EnItem00* Obj_Bean_Item_DropCollectible_Hack(z64_game_t* play, z64_actor_t* actor, int16_t num) {
    // Build the complete flag for resolve alt flag
    ActorAdditionalData* extras = Actor_GetAdditionalData(actor);
    xflag_t flag = { 0 };
    Actor_BuildFlag(actor, &flag, extras->actor_id, num+1);
    flag = resolve_alternative_flag(&flag);

    // Check for override
    override_t override = get_newflag_override(&flag);
    if(override.key.all) {
        // Spawn fairy directly and set override and flag
        spawn_actor_with_flag = &flag;
        EnElf* spawned = (EnElf*)z64_SpawnActor(&play->actor_ctxt, play, ACTOR_EN_ELF, actor->pos_world.x, actor->pos_world.y + 15.0f, actor->pos_world.z, 0,0,0, 6 );
        spawn_actor_with_flag = NULL;
        spawned->override = override;
        return (EnItem00*)spawned; // Casting just to suppress warning. The spawned actor is not an EnItem00. Same problem in vanilla code
    }

    // No override so just call original function
    z64_xyzf_t spawnPos;
    spawnPos.x = actor->pos_world.x;
    spawnPos.y = actor->pos_world.y - 25.0f;
    spawnPos.z = actor->pos_world.z;
    return z64_Item_DropCollectible(play, &spawnPos, ITEM00_FLEXIBLE);
}


/*
extern void OVL_ObjBean_Init(z64_actor_t* thisx, z64_game_t* play);

void ObjBean_Init_Hook(z64_actor_t* thisx, z64_game_t* play) {
    // Call original init function
    // Resolve overlay address
    ActorFunc ObjBean_Init = resolve_overlay_addr(&OVL_ObjBean_Init, thisx->actor_id);
    ObjBean_Init(thisx, play);
    
    // Build and store flag in actor
    ActorAdditionalData* extras = Actor_GetAdditionalData(thisx);
    Actor_BuildFlag(thisx, &extras->flag, extras->actor_id, 0);
}
*/