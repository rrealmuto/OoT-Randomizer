#include "z64.h"
#include "get_items.h"
#include "scene.h"
#include "actor.h"
#include "models.h"
#include "fishing.h"
#include "item_table.h"
#include "util.h"
#include "save.h"

extern uint8_t SHUFFLE_FISHIES;
extern xflag_t* spawn_actor_with_flag;
        //0x80a44a50 - sFishOnHandLength
        //0x80a47eec - sFishOnHandIsLoach
        //0x80a47eee - sLureCaughtWith
#define pFishOnHandLength_VRAM (void*)0x80a44a50
#define pFishOnHandIsLoach_VRAM (void*)0x80a47eec
#define pFishingPlayState_VRAM (void*)0x80a47f1c
#define Fishing_HandleOwnerDialog_ADDR (void*)0x80a419dc

Fishing* caught_fish = NULL;
int32_t fishing_get_item_id = 0;

z64_actor_t* Fishing_Actor_Spawn_Hook(void* actorCtx, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int16_t rotX, int16_t rotY, int16_t rotZ, int16_t params) {
    if(SHUFFLE_FISHIES) {
        xflag_t fish_flag = { 0 };
        fish_flag.room = globalCtx->room_index;
        fish_flag.scene = globalCtx->scene_index;
        fish_flag.setup = curr_scene_setup;
        fish_flag.flag = globalCtx->link_age; // Use the current age as the flag to distinguish child/adult fish
        fish_flag.subflag = params - 100; // Params contains the value 100 + i from the spawn loop. Add 1
        override_t override = get_newflag_override_by_flag(&fish_flag, globalCtx);
        if(override.key.all) {
            spawn_actor_with_flag = &fish_flag;
            Fishing* fish = z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
            fish->override = override;
            spawn_actor_with_flag = NULL;
            return fish;
        }
        return NULL;
    }
    return z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
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
        ActorAdditionalData* extras = Actor_GetAdditionalData(this);
        uint16_t resolved_item_id = resolve_upgrades(fish->override);
        item_row_t* item_row = get_item_row(resolved_item_id);
        Set_NewOverrideFlag(&(extras->flag));
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
        globalCtx->msgContext.choiceIndex = 0;
        z64_DisplayTextbox(globalCtx, item_row->text_id, NULL);
        return;
    }
    z64_DisplayTextbox(globalCtx, messageID, NULL);
}

typedef void (*Fishing_HandleOwnerDialog_Func)(Fishing* this, z64_game_t* globalCtx);

void Fishing_HandleOwnerDialog_Hook(Fishing* this, z64_game_t* globalCtx) {
    int16_t* pFishingPlayState = (int16_t*)resolve_overlay_addr(pFishingPlayState_VRAM, 0xFE);
    Fishing_HandleOwnerDialog_Func Fishing_HandleOwnerDialog = (Fishing_HandleOwnerDialog_Func)resolve_overlay_addr(Fishing_HandleOwnerDialog_ADDR, 0xFE);
    if(SHUFFLE_FISHIES) {
        if(*pFishingPlayState == 0) {
            switch(this->stateAndTimer) {
                case 0:
                    if(*pFishingPlayState == 0) {
                        this->actor.text_id = 0x407B;
                    }

                    if(Actor_TalkOfferAccepted(&this->actor, globalCtx)) {
                        this->stateAndTimer = 1;
                    }
                    else {
                        Actor_OfferTalk(&this->actor, globalCtx, 100.0);
                    }

                    break;
                case 1: // Advance message box based on rod and fish state
                    if(Message_GetState(&globalCtx->msgContext) == TEXT_STATE_DONE && Message_ShouldAdvance(globalCtx)) {
                        // First, check if we have already received the reward for the current age:
                        if(LINK_IS_ADULT && !((HIGH_SCORE(HS_FISHING) & HS_FISH_PRIZE_ADULT) > 0) && (extended_savectx.largest_fish_found[0] >= 13)) {
                            Message_ContinueTextbox(globalCtx, 0x407E);
                            this->stateAndTimer = 4;
                        }
                        else if(LINK_IS_CHILD && !((HIGH_SCORE(HS_FISHING) & HS_FISH_PRIZE_CHILD) > 0) && (extended_savectx.largest_fish_found[1] >= 9)) {
                            Message_ContinueTextbox(globalCtx, 0x407E);
                            this->stateAndTimer = 5;
                        }
                        else if(extended_savectx.has_loach && !((HIGH_SCORE(HS_FISHING) & HS_FISH_PRIZE_LOACH))) {
                            Message_ContinueTextbox(globalCtx, 0x409B);
                            this->stateAndTimer = 6;
                        }
                        else if(extended_savectx.has_fishing_rod) {
                            Message_ContinueTextbox(globalCtx, 0x407C);
                            this->stateAndTimer = 2;
                        }
                        else { // No fishing rod
                            Message_ContinueTextbox(globalCtx, 0x407D);
                            this->stateAndTimer = 3;
                        }
                    }
                    break;
                case 2: // Has fishing rod
                    if(Message_GetState(&globalCtx->msgContext) == TEXT_STATE_CHOICE && Message_ShouldAdvance(globalCtx)) {
                        Message_CloseTextbox(globalCtx);
                        if(globalCtx->msgContext.choiceIndex == 0) {
                            // Chose to fish. Set up fishing game
                            
                            globalCtx->unk_interfacectx_260_fishing = 1; // This is what sets the interface to "fishing" mode and lets you use the pole
                            globalCtx->startPlayerFishing(globalCtx); // This does something
                            *pFishingPlayState = 1;  // Tell's the Fishing actors that we're fishing
                            this->stateAndTimer = 0; // Reset this so that the text gets handled normally.
                        } else {
                            // Chose not to fish
                            this->stateAndTimer = 0;
                        }
                    }
                    break;
                case 3: // No fishing rod
                    if(Message_GetState(&globalCtx->msgContext) == TEXT_STATE_DONE && Message_ShouldAdvance(globalCtx)) {
                        Message_CloseTextbox(globalCtx);
                        this->stateAndTimer = 0;
                    }
                    break;
                case 4: // Adult prize
                    if(((Message_GetState(&globalCtx->msgContext) == TEXT_STATE_EVENT) || (Message_GetState(&globalCtx->msgContext) == TEXT_STATE_NONE)) && Message_ShouldAdvance(globalCtx)) {
                        Message_CloseTextbox(globalCtx);
                        this->actor.parent = NULL;
                        HIGH_SCORE(HS_FISHING) |= HS_FISH_PRIZE_ADULT;
                        // Give adult reward
                        fishing_get_item_id = 56;
                        //Actor_OfferGetItem(&this->actor, globalCtx, 56, 2000.0f, 1000.0f);
                        this->stateAndTimer = 7;
                    }
                    break;
                case 5: // Child prize
                    if(((Message_GetState(&globalCtx->msgContext) == TEXT_STATE_EVENT) || (Message_GetState(&globalCtx->msgContext) == TEXT_STATE_NONE)) && Message_ShouldAdvance(globalCtx)) {
                        Message_CloseTextbox(globalCtx);
                        this->actor.parent = NULL;
                        HIGH_SCORE(HS_FISHING) |= HS_FISH_PRIZE_CHILD;
                        // Give child reward
                        fishing_get_item_id = 62;
                        //Actor_OfferGetItem(&this->actor, globalCtx, 62, 2000.0f, 1000.0f);
                        this->stateAndTimer = 7;
                    }
                    break;
                case 6: // Loach prize
                    if(((Message_GetState(&globalCtx->msgContext) == TEXT_STATE_EVENT) || (Message_GetState(&globalCtx->msgContext) == TEXT_STATE_NONE)) && Message_ShouldAdvance(globalCtx)) {
                        Message_CloseTextbox(globalCtx);
                        this->actor.parent = NULL;
                        HIGH_SCORE(HS_FISHING) |= HS_FISH_PRIZE_LOACH;
                        fishing_get_item_id = 0x56;
                        //Actor_OfferGetItem(&this->actor, globalCtx, 0x56, 2000.0f, 1000.0f);
                        this->stateAndTimer = 7;
                    }
                    break;
                case 7: // GI loop? Idk but this is kinda what the original code does. Seems to work. Otherwise sometimes it doesn't give the item.
                    if(this->actor.parent != NULL) {
                        this->stateAndTimer = 8;
                        fishing_get_item_id = 0;
                    } else {
                        Actor_OfferGetItem(&this->actor, globalCtx, fishing_get_item_id, 2000.0f, 1000.0f);
                    }
                    break;
                case 8: // Wait to close get_item message box
                    if((Message_GetState(&globalCtx->msgContext) == TEXT_STATE_DONE) && Message_ShouldAdvance(globalCtx)) {
                        this->stateAndTimer = 0;
                    }
                    break;
            }
        }
        else {
            Fishing_HandleOwnerDialog(this, globalCtx);
        }
        
    }
    else {
        
        Fishing_HandleOwnerDialog(this, globalCtx);
    }
    
}