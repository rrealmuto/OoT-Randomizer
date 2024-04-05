#include "z64.h"
#include "player.h"
#include "util.h"
#include "models.h"

#define Player_DrawGameplay_Orig_Addr 0x80848200 // Original Player_DrawGameplay function which we'll call
#define sMaskDlists_Addr 0x808526fc // sMaskDlists contains the displaylist to call for each mask

extern int32_t Player_OverrideLimbDrawGameplayDefault(z64_game_t* play, int32_t limbIndex, Gfx** dList, z64_xyzf_t* pos, z64_xyz_t* rot, void*);

Gfx* sMaskDlistsAdult[] = {0,0,0,0x06001010, 0,0,0,0};

// Hook Player_DrawGameplay so we can draw our new bunny hood
void Player_DrawGameplay_Hook(z64_game_t* globalCtx, z64_link_t* this, int32_t lod, Gfx* cullDList, OverrideLimbDrawOpa overrideLimbDraw) {
    
    Player_DrawGameplay_Func Player_DrawGameplay = resolve_player_ovl_addr(Player_DrawGameplay_Orig_Addr);
    Player_DrawGameplay(globalCtx, this, lod, cullDList, overrideLimbDraw);
    z64_gfx_t* gfx = globalCtx->common.gfx;
    if(overrideLimbDraw == Player_OverrideLimbDrawGameplayDefault && this->current_mask)
    {
        uint32_t* sMaskDlists;
        if(LINK_IS_ADULT) {
            sMaskDlists = sMaskDlistsAdult;
            if(this->current_mask == PLAYER_MASK_BUNNY) {
                // If adult is wearing bunny hood, set the object segment to the bunny hood object which is kept in RAM.
                gSPSegment(gfx->poly_opa.p++, 0x06, bunny_hood_obj.buf);
            }
        }
        else {
             sMaskDlists = (uint32_t*)resolve_player_ovl_addr(sMaskDlists_Addr);
        }
        // Call the correct mask dlist
        if (sMaskDlists[this->current_mask - 1]) {
            gSPDisplayList(gfx->poly_opa.p++, sMaskDlists[this->current_mask - 1]);
        }
        // Reset the object segment 0x06
        gSPSegment(gfx->poly_opa.p++, 0x06, globalCtx->obj_ctxt.objects[this->common.obj_bank_index].data);
    }
}