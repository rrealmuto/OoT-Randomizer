#include "z64.h"

#define GAMEPLAY_FIELD_KEEP_BEEHIVE_TEXTURE (uint8_t*)0x05008900

// Hack beehives to drop a collectible w / an extended flag, based on the grotto param ?
void obj_comb_drop_collectible(z64_actor_t *actor, int16_t params)
{
    // Check if we're in a grotto
    uint8_t flag = actor->rot_init.z;
    if (z64_game.scene_index == 0x3E)
    {
        // We're in a grotto so offset by 2x grotto id. The Rz flags for the grottos need to be set to 0/1 beforehand.
        flag = (2 * (z64_file.grotto_id & 0x1F)) + flag;
        // and add 0x60
        flag += 0x60;
    }

    if (params >= 0)
    {
        // set up params for Item_DropCollectible
        params |= ((flag & 0x3F) << 8) + (flag & 0xC0);
        z64_Item_DropCollectible2(&z64_game, &actor->pos_2, params);
    }
}

void ObjComb_Draw_Hack(z64_actor_t* this, z64_game_t* game)
{
    uint8_t *texture = GAMEPLAY_FIELD_KEEP_BEEHIVE_TEXTURE;
    z64_gfx_t *gfx = game->common.gfx;
    gfx->poly_opa.d -= 2;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gSPEndDisplayList(gfx->poly_opa.d + 1);
    gMoveWd(gfx->poly_opa.p++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);
}