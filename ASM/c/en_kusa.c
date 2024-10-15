#include "z64.h"
#include "en_kusa.h"
#include "actor.h"
#include "get_items.h"
#include "textures.h"
#include "item_table.h"


void* EnKusa_Draw = 0x80a80a50;

// Hacked call at the beginning of enkusa_dropcollectible
// Use this for our grass shuffle drops
// Return 0 if we didn't override so it returns to the original EnKusa_DropCollectible function
// Return 1 if we overrode so it returns to the caller
int enkusa_dropcollectible_hack(z64_actor_t* actor, z64_game_t* game)
{
    // Get our flag from the actor
    ActorAdditionalData* extra = Actor_GetAdditionalData(actor);
    if(extra->flag.all && !Get_NewFlag(&(extra->flag))) {
        drop_collectible_override_flag = extra->flag;
        z64_Item_DropCollectible(game, &(actor->pos_world), 0);
        z64_bzero(&drop_collectible_override_flag, sizeof(drop_collectible_override_flag));
        return 1;
    }
    return 0;
}

void EnKusa_Draw_Hack(z64_actor_t* actor, z64_game_t* game) {
    //
    // Resolve original function from overlay
    // Figure out which texture to use
    // Original texture for big grass is 0x0500B140
    // Pick the original texture based on grass type
    EnKusa* this = (EnKusa*)actor;
    static uint8_t* textures[] = { 0x0500B140, 0x04035BD0, 0x04035BD0 };
    uint8_t* grass_texture_custom = get_texture(TEXTURE_ID_GRASS_CUSTOM);
    uint8_t* grass_small_texture_custom = get_texture(TEXTURE_ID_GRASS_SMALL_CUSTOM);
    uint8_t* custom_textures[] = {grass_texture_custom, grass_small_texture_custom, grass_small_texture_custom };
    uint8_t* texture = textures[actor->variable & 0x03];
    colorRGBA8_t color = {.r = 0xFF, .g = 0xFF, .b = 0xFF, .a = 0xFF};

    // Check if the item has already been collected to properly redraw the color
    ActorAdditionalData* extras = Actor_GetAdditionalData(actor);
    if(extras->flag.all && Get_NewFlag(&(extras->flag))) {
        // Flag is set so clear the chest type
        this->chest_type = -1;
        extras->flag.all = 0;
    }

    // Check CTMC
    switch (this->chest_type) {
        case GILDED_CHEST:
            texture = custom_textures[actor->variable & 0x03];
            color.r = 255;
            color.g = 243;
            color.b = 0;
            break;

        case SILVER_CHEST:
            texture = custom_textures[actor->variable & 0x03];
            color.r = 176;
            color.g = 176;
            color.b = 176;
            break;

        case GOLD_CHEST:
            texture = custom_textures[actor->variable & 0x03];
            color.r = 0;
            color.g = 0;
            color.b = 255;
            break;

        case SKULL_CHEST_SMALL:
        case SKULL_CHEST_BIG:
            texture = custom_textures[actor->variable & 0x03];
            color.r = 46;
            color.g = 46;
            color.b = 46;
            break;

        case HEART_CHEST_SMALL:
        case HEART_CHEST_BIG:
            texture = custom_textures[actor->variable & 0x03];
            color.r = 255;
            color.g = 51;
            color.b = 163;
            break;

        default:

            break;
    }
    z64_gfx_t *gfx = game->common.gfx;

    // push custom dlist (that sets the texture) to segment 09
    gfx->poly_opa.d -= 2;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gSPEndDisplayList(gfx->poly_opa.d + 1);
    gMoveWd(gfx->poly_opa.p++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);
    gDPSetPrimColor(gfx->poly_opa.p++, 0,0, color.r,color.g,color.b, color.a);
    ActorFunc func = (ActorFunc)Actor_ResolveOverlayAddr(actor, EnKusa_Draw);
    func(actor,game);
}

/*
in field_keep
gFieldBushDL 0x0500b9d0
{
    00000000: E7000000 00000000  gsDPPipeSync(),
    00000008: E3001001 00000000  gsDPSetTextureLUT(G_TT_NONE),
    00000010: D7000002 FFFFFFFF  gsSPTexture(qu016(0.999985), qu016(0.999985), 0, G_TX_RENDERTILE, G_ON),
    00000018: FD100000 0500B140  gsDPLoadTextureBlock(0x0500B140, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    00000020: F5100000 07094250
    00000028: E6000000 00000000
    00000030: F3000000 073FF100
    00000038: E7000000 00000000
    00000040: F5101000 00094250
    00000048: F2000000 0007C07C
    00000050: FC127E03 FFFFF3F8  gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    00000058: E200001C C8113078  gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    00000060: D9F3FBFF 00000000  gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    00000068: D9FFFFFF 00030000  gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    00000070: FA000000 FFFFFFFF  gsDPSetPrimColor(0, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    00000078: 01009012 0500B940  gsSPVertex(0x0500B940, 9, 0),
    00000080: 06000204 0006080A  gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    00000088: 050C0E10 00000000  gsSP1Triangle(6, 7, 8, 0),
    00000090: DF000000 00000000  gsSPEndDisplayList(),
}
*/

/* in object_kusa
{
    object_kusa_DL_000140 - small cuttable grass
    00000000: E7000000 00000000 gsDPPipeSync(),
    00000008: E3001001 00000000 gsDPSetTextureLUT(G_TT_NONE),
    00000010: D7000002 FFFFFFFF gsSPTexture(qu016(0.999985), qu016(0.999985), 0, G_TX_RENDERTILE, G_ON),
    00000018: FD100000 04035BD0 gsDPLoadTextureBlock(0x04035BD0, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    00000020: F5100000 070D4350
    00000028: E6000000 00000000
    00000030: F3000000 073FF100
    00000038: E7000000 00000000
    00000040: F5101000 000D4350
    00000048: F2000000 0007C07C
    00000050: FC127E03 FFFFF3F8 gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    00000058: E200001C C8113078 gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    00000060: D9F3FBFF 00000000 gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    00000068: D9FFFFFF 00030000 gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    00000070: FA000000 FFFFFFFF gsDPSetPrimColor(0, 0, 0xFF, 0xFF, 0xFF, 0xFF),
    00000078: 01014028 06000000 gsSPVertex(0x06000000, 20, 0),
    00000080: 06000204 00000602 gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    00000088: 06080A0C 00080E0A gsSP2Triangles(4, 5, 6, 0, 4, 7, 5, 0),
    00000090: 06101214 00101612 gsSP2Triangles(8, 9, 10, 0, 8, 11, 9, 0),
    00000098: 06181A1C 00181C1E gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    000000A0: 06202224 00202426 gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    000000A8: DF000000 00000000 gsSPEndDisplayList(),
}
*/
