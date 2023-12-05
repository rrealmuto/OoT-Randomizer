#include "z64.h"
#include "get_items.h"
#include "scene.h"
#include "actor.h"
#include "models.h"
#include "fishing.h"
#include "item_table.h"
#include "util.h"

extern xflag_t* spawn_actor_with_flag;
        //0x80a44a50 - sFishOnHandLength
        //0x80a47eec - sFishOnHandIsLoach
        //0x80a47eee - sLureCaughtWith
#define pFishOnHandLength_VRAM (void*)0x80a44a50
#define pFishOnHandIsLoach_VRAM (void*)0x80a47eec

Fishing* caught_fish = NULL;

z64_actor_t* Fishing_Actor_Spawn_Hook(void* actorCtx, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int16_t rotX, int16_t rotY, int16_t rotZ, int16_t params) {
    xflag_t fish_flag = { 0 };
    fish_flag.room = globalCtx->room_index;
    fish_flag.scene = globalCtx->scene_index;
    fish_flag.setup = curr_scene_setup;
    fish_flag.flag = globalCtx->link_age; // Use the current age as the flag to distinguish child/adult fish
    fish_flag.subflag = params - 100; // Params contains the value 100 + i from the spawn loop. Add 1
    spawn_actor_with_flag = &fish_flag;
    Fishing* fish = z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
    fish->override = get_newflag_override(&fish->actor, globalCtx);
    spawn_actor_with_flag = NULL;
    return fish;
}

extern void Fishing_DrawFish(z64_actor_t* this, z64_game_t* globalCtx);
void Fishing_SkeletonDraw_Hook(z64_game_t* globalCtx, void** skeleton, z64_xyz_t* jointTable, int32_t dListCount, OverrideLimbDrawOpa overrideLimbDraw, PostLimbDrawOpa postLimbDraw, void* this) {
    Fishing* fish = (Fishing*)this;
    if(fish->override.key.all) {
        postLimbDraw(globalCtx, fish->isLoach ? 0x0B : 0x0D, NULL, NULL, this); // Call the postLimbDraw function. This is used to set the mouth position which affects where the lure moves.
        fishing_draw(&(fish->actor), globalCtx);
        return;
    }
    SkelAnime_DrawFlexOpa(globalCtx, skeleton, jointTable, dListCount, overrideLimbDraw, postLimbDraw, this);
    //Fishing_DrawFish(this, globalCtx);
}

void Fishing_GiveOverride_Kill(z64_actor_t* this) {
    Fishing* fish = (Fishing*)this;
    if(fish->override.key.all) {
        uint16_t resolved_item_id = resolve_upgrades(fish->override);
        item_row_t* item_row = get_item_row(resolved_item_id);
        dispatch_item(resolved_item_id, fish->override.value.base.player, &(fish->override), item_row);
        // Reset the fishing variables that keep track if we have caught a fish
        float* pFishOnHandLength = (float*)resolve_overlay_addr(pFishOnHandLength_VRAM, 0xFE);
        uint8_t* pFishOnHandIsLoach = (float*)resolve_overlay_addr(pFishOnHandIsLoach_VRAM, 0xFE);
        *pFishOnHandIsLoach = 0;
        *pFishOnHandLength = 0.0;
    }
    caught_fish = NULL;
    z64_ActorKill(this);
}

void Fishing_CaughtFish_Textbox(z64_game_t* globalCtx, uint16_t messageID, z64_actor_t* actor) {
    if(caught_fish && caught_fish->override.key.all) {
        // Get the message ID from the override
        uint16_t resolved_item_id = resolve_upgrades(caught_fish->override);
        item_row_t* item_row = get_item_row(resolved_item_id);
        z64_DisplayTextbox(globalCtx, item_row->text_id, NULL);
        return;
    }
    z64_DisplayTextbox(globalCtx, messageID, NULL);
}