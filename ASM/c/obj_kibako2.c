#include "obj_kibako2.h"

#define CRATE_TOP_TEXTURE 0x06000020
#define CRATE_SIDE_TEXTURE 0x06000420
#define CRATE_PALETTE 0x06000000

#define CRATE_DLIST 0x06000960

extern bool POTCRATE_TEXTURES_MATCH_CONTENTS;

extern void func_8007E298(z64_gfx_t* gfx);
asm(".equ func_8007E298, 0x8007E298");
extern Mtx_t *write_matrix_stack_top(z64_gfx_t *gfx);
asm(".equ write_matrix_stack_top, 0x800AB900");

//Hacks the regular crate spawn collectible function to use more flag space
//The additional flag info is stored in the actors dropFlag variable (unused by collectibles)

void ObjKibako2_SpawnCollectible_Hack(ObjKibako2* this, z64_game_t* globalCtx) {
    int16_t itemDropped;
    int16_t collectibleFlagTemp;

    collectibleFlagTemp = this->collectibleFlag & 0x3F; //Get the vanilla part of the collectible flag
    uint16_t extendedCollectibleFlag = (this->collectibleFlag & 0x00C0); //Get the upper part of the collectible flag that we'll store elsewhere
    itemDropped = this->dyna.actor.rot_init.x & 0x1F;
    if (itemDropped >= 0 && itemDropped < 0x1A) {
        EnItem00* spawned = z64_Item_DropCollectible(globalCtx, &this->dyna.actor.pos_2, itemDropped | (collectibleFlagTemp << 8) | extendedCollectibleFlag);
        //spawned->actor.dropFlag |= (extendedCollectibleFlag << 1) & 0xFE; 
    }
}

extern void *CRATE_TOP_TEXTURE_DEFAULT;
extern void *CRATE_TOP_TEXTURE_GOLD;
extern void *CRATE_TOP_TEXTURE_WEB;
extern void *CRATE_TOP_TEXTURE_BOSSKEY;

extern void *CRATE_SIDE_TEXTURE_DEFAULT;
extern void *CRATE_SIDE_TEXTURE_GOLD;
extern void *CRATE_SIDE_TEXTURE_WEB;
extern void *CRATE_SIDE_TEXTURE_BOSSKEY;

extern void* CRATE_PALETTE_DEFAULT;
extern void *CRATE_PALETTE_GOLD;
extern void *CRATE_PALETTE_SILVER;
extern void *CRATE_PALETTE_WEB;
extern void *CRATE_PALETTE_BOSSKEY;

override_t get_crate_override(z64_actor_t *actor, z64_game_t *game) {
    //make a dummy EnItem00 with enough info to get the override
    ObjKibako2* this = (ObjKibako2*)actor;
    EnItem00 dummy;
    dummy.collectibleFlag = (this->collectibleFlag & 0x3F) | (this->collectibleFlag & 0x00C0); 
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;

    if (!should_override_collectible(&dummy)) {
        return (override_t){0};
    }

    return lookup_override(&dummy, game->scene_index, 0);
}

void ObjKibako2_Draw(z64_actor_t* actor, z64_game_t* game)
{
    
    z64_gfx_t *gfx = game->common.gfx;

    func_8007E298(gfx);
    //write matrix
    Mtx_t *mtx = write_matrix_stack_top(gfx);
    gSPMatrix(gfx->poly_opa.p++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD);


    //get dlist and texture
    int dlist = CRATE_DLIST;
    void* palette = &CRATE_PALETTE_DEFAULT;
    void* top_texture = &CRATE_TOP_TEXTURE_DEFAULT;
    void* side_texture = &CRATE_SIDE_TEXTURE_DEFAULT;
    
    override_t crate_override = get_crate_override(actor, game);
    if(POTCRATE_TEXTURES_MATCH_CONTENTS && crate_override.key.all != 0)
    {
        uint16_t itemID = resolve_upgrades(crate_override.value.item_id);
        item_row_t* row = get_item_row(itemID);
        if(row->chest_type == GILDED_CHEST)
        {
            palette = &CRATE_PALETTE_GOLD;
            top_texture = &CRATE_TOP_TEXTURE_GOLD;
            side_texture = &CRATE_SIDE_TEXTURE_GOLD;
        }
        else if(row->chest_type == SILVER_CHEST)
        {
            palette = &CRATE_PALETTE_SILVER;
            top_texture = &CRATE_TOP_TEXTURE_GOLD;
            side_texture = &CRATE_SIDE_TEXTURE_GOLD;
        }
        else if(row->chest_type == GOLD_CHEST)
        {
            palette = &CRATE_PALETTE_BOSSKEY;
            top_texture = &CRATE_TOP_TEXTURE_BOSSKEY;
            side_texture = &CRATE_SIDE_TEXTURE_BOSSKEY;
        }
        else if(row->chest_type == SKULL_CHEST_SMALL || row->chest_type == SKULL_CHEST_BIG)
        {
            palette = &CRATE_PALETTE_WEB;
            top_texture = &CRATE_TOP_TEXTURE_WEB;
            side_texture = &CRATE_SIDE_TEXTURE_WEB;
        }
    }

    gfx->poly_opa.d -= 6;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_CI, G_IM_SIZ_8b, 1, top_texture);
    gSPEndDisplayList(gfx->poly_opa.d + 1);
    gDPSetTextureImage(gfx->poly_opa.d + 2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, palette);
    gSPEndDisplayList(gfx->poly_opa.d + 3);
    gDPSetTextureImage(gfx->poly_opa.d + 4, G_IM_FMT_CI, G_IM_SIZ_8b, 1, side_texture);
    gSPEndDisplayList(gfx->poly_opa.d + 5);

    gMoveWd(gfx->poly_opa.p++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);

    gSPDisplayList(gfx->poly_opa.p++, dlist);
}