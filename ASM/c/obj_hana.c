#include "z64.h"

static void* ObjHana_Draw = 0x80abc1c8;
static uint8_t* texture = 0x0500B140;

void ObjHana_Draw_Hack(z64_actor_t* actor, z64_game_t* game) {
    // If it's the grass object we need to set up segment 9 and call setprimcolor
    if((actor->variable & 0x03) == 2) {
        z64_gfx_t *gfx = game->common.gfx;

        // push custom dlist (that sets the texture) to segment 09
        gfx->poly_opa.d -= 2;
        gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
        gSPEndDisplayList(gfx->poly_opa.d + 1);
        gMoveWd(gfx->poly_opa.p++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);
        gDPSetPrimColor(gfx->poly_opa.p++, 0,0, 0xFF,0xFF,0xFF,0xFF);
    }

    ActorFunc func = (ActorFunc)Actor_ResolveOverlayAddr(actor, ObjHana_Draw);
    func(actor,game);
}
