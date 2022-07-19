#include "obj_kibako2.h"
#include "textures.h"
#define CRATE_DLIST (z64_gfx_t *)0x06000960

#define CRATE_CI8_TEXTURE_PALETTE_OFFSET 0x00
#define CRATE_CI8_TEXTURE_TOP_OFFSET 0x200
#define CRATE_CI8_TEXTURE_SIDE_OFFSET 0xA00


extern uint8_t POTCRATE_TEXTURES_MATCH_CONTENTS;

// Hacks the regular crate spawn collectible function to use more flag space
// The additional flag info is stored in the actors dropFlag variable (unused by collectibles)

void ObjKibako2_SpawnCollectible_Hack(ObjKibako2 *this, z64_game_t *globalCtx)
{
    int16_t itemDropped;
    int16_t collectibleFlagTemp;

    collectibleFlagTemp = this->collectibleFlag & 0x3F;                  // Get the vanilla part of the collectible flag
    uint16_t extendedCollectibleFlag = (this->collectibleFlag & 0x00C0); // Get the upper part of the collectible flag that we'll store elsewhere
    itemDropped = this->dyna.actor.rot_init.x & 0x1F;
    if (itemDropped >= 0 && itemDropped < 0x1A)
    {
        EnItem00 *spawned = z64_Item_DropCollectible(globalCtx, &this->dyna.actor.pos_2, itemDropped | (collectibleFlagTemp << 8) | extendedCollectibleFlag);
        // spawned->actor.dropFlag |= (extendedCollectibleFlag << 1) & 0xFE;
    }
}

override_t get_crate_override(z64_actor_t *actor, z64_game_t *game)
{
    // make a dummy EnItem00 with enough info to get the override
    ObjKibako2 *this = (ObjKibako2 *)actor;
    EnItem00 dummy;
    dummy.collectibleFlag = (this->collectibleFlag & 0x3F) | (this->collectibleFlag & 0x00C0);
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;
    dummy.actor.variable = 0;
    if (!should_override_collectible(&dummy))
    {
        return (override_t){0};
    }

    return lookup_override((z64_actor_t *)&dummy, game->scene_index, 0);
}

void ObjKibako2_Draw(z64_actor_t *actor, z64_game_t *game)
{
    uint8_t* texture = get_texture(TEXTURE_ID_CRATE_DEFAULT);
    // get original palette and textures
    //uint64_t *palette = get_texture(TEXTURE_ID_CRATE_PALETTE_DEFAULT);
    //uint64_t *top_texture = get_texture(TEXTURE_ID_CRATE_TOP_DEFAULT);
    //uint64_t *side_texture = get_texture(TEXTURE_ID_CRATE_SIDE_DEFAULT);

    // get override palette and textures
    override_t crate_override = get_crate_override(actor, game);
    if(POTCRATE_TEXTURES_MATCH_CONTENTS == PTMC_UNCHECKED && crate_override.key.all != 0)
    {
        texture = get_texture(TEXTURE_ID_CRATE_GOLD);
    }
    else if (POTCRATE_TEXTURES_MATCH_CONTENTS == PTMC_CONTENTS && crate_override.key.all != 0)
    {
        uint16_t item_id = resolve_upgrades(crate_override.value.item_id);
        item_row_t *row = get_item_row(crate_override.value.looks_like_item_id);
        if (row == NULL) {
            row = get_item_row(crate_override.value.item_id);
        }
        if (row->chest_type == GILDED_CHEST)
        {
            texture = get_texture(TEXTURE_ID_CRATE_GOLD);
            //palette = get_texture(TEXTURE_ID_CRATE_PALETTE_GOLD);
            //top_texture = get_texture(TEXTURE_ID_CRATE_TOP_GOLD);
            //side_texture = get_texture(TEXTURE_ID_CRATE_SIDE_GOLD);
        }
        else if (row->chest_type == SILVER_CHEST)
        {
            texture = get_texture(TEXTURE_ID_CRATE_KEY);
            //palette = get_texture(TEXTURE_ID_CRATE_PALETTE_KEY);
            //top_texture = get_texture(TEXTURE_ID_CRATE_TOP_KEY);
            //side_texture = get_texture(TEXTURE_ID_CRATE_SIDE_KEY);
        }
        else if (row->chest_type == GOLD_CHEST)
        {
            texture = get_texture(TEXTURE_ID_CRATE_BOSSKEY);
            //palette = get_texture(TEXTURE_ID_CRATE_PALETTE_BOSSKEY);
            //top_texture = get_texture(TEXTURE_ID_CRATE_TOP_BOSSKEY);
            //side_texture = get_texture(TEXTURE_ID_CRATE_SIDE_BOSSKEY);
        }
        else if (row->chest_type == SKULL_CHEST_SMALL || row->chest_type == SKULL_CHEST_BIG)
        {
            texture = get_texture(TEXTURE_ID_CRATE_SKULL);
            //palette = get_texture(TEXTURE_ID_CRATE_PALETTE_SKULL);
            //top_texture = get_texture(TEXTURE_ID_CRATE_TOP_SKULL);
            //side_texture = get_texture(TEXTURE_ID_CRATE_SIDE_SKULL);
        }
    }

    // push custom dlists (that set the palette and textures) to segment 09
    z64_gfx_t *gfx = game->common.gfx;
    gfx->poly_opa.d -= 6;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_CI, G_IM_SIZ_16b, 1, texture + CRATE_CI8_TEXTURE_TOP_OFFSET);
    gSPEndDisplayList(gfx->poly_opa.d + 1);
    gDPSetTextureImage(gfx->poly_opa.d + 2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture + CRATE_CI8_TEXTURE_PALETTE_OFFSET);
    gSPEndDisplayList(gfx->poly_opa.d + 3);
    gDPSetTextureImage(gfx->poly_opa.d + 4, G_IM_FMT_CI, G_IM_SIZ_16b, 1, texture + CRATE_CI8_TEXTURE_SIDE_OFFSET);
    gSPEndDisplayList(gfx->poly_opa.d + 5);

    gMoveWd(gfx->poly_opa.p++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);

    // draw the original dlist that has been hacked in ASM to jump to the custom dlists
    z64_Gfx_DrawDListOpa(game, CRATE_DLIST);
}
