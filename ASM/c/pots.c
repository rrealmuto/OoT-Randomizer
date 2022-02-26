#include "pots.h"
#include "n64.h"
#include "gfx.h"

#define DUNGEON_POT_SIDE_TEXTURE 0x050108A0
#define DUNGEON_POT_DLIST 0x05017870

#define POT_SIDE_TEXTURE 0x06000000
#define POT_DLIST 0x060017C0

uint32_t POT_TEXTURE_MATCH_CONTENTS = 0;

extern void *GILDED_CHEST_FRONT_TEXTURE;
extern void *SILVER_CHEST_FRONT_TEXTURE;
extern void *SKULL_CHEST_FRONT_TEXTURE;

extern Mtx_t *write_matrix_stack_top(z64_gfx_t *gfx);
asm(".equ write_matrix_stack_top, 0x800AB900");

override_t get_pot_override(z64_actor_t *actor, z64_game_t *game) {
    //make a dummy EnItem00 with enough info to get the override
    EnItem00 dummy;
    dummy.collectibleFlag = (actor->variable & 0x7E00) >> 8;
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;

    return lookup_override(&dummy, game->scene_index, 0);
}

void draw_pot(z64_actor_t *actor, z64_game_t *game) {
    z64_gfx_t *gfx = game->common.gfx;
    Gfx *opa_ptr = gfx->poly_opa.p;

    //write matrix
    Mtx_t *mtx = write_matrix_stack_top(gfx);

    //get chest_type
    override_t override = get_pot_override(actor, game);
    uint16_t resolved_item_id = resolve_upgrades(override.value.item_id);
    item_row_t *item_row = get_item_row(resolved_item_id);
    uint8_t chest_type = item_row->chest_type;

    gSPMatrix(opa_ptr++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD);

    //get dlist and texture
    int dlist = DUNGEON_POT_DLIST;
    void *sideTexture = (void *)DUNGEON_POT_SIDE_TEXTURE;

    if ((actor->variable >> 8) & 1) {
        dlist = POT_DLIST;
        sideTexture = POT_SIDE_TEXTURE;
    }

    if (POT_TEXTURE_MATCH_CONTENTS) {
        if (chest_type == GILDED_CHEST) {
            sideTexture = &GILDED_CHEST_FRONT_TEXTURE;
        }
        else if (chest_type == SILVER_CHEST || chest_type == GOLD_CHEST) {
            sideTexture = &SILVER_CHEST_FRONT_TEXTURE;
        }
        else if (chest_type == SKULL_CHEST_SMALL || chest_type == SKULL_CHEST_BIG) {
            sideTexture = &SKULL_CHEST_FRONT_TEXTURE;
        }
    }

    //modify the pot display list with custom textures
    gfx->poly_opa.d -= 2;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, sideTexture);
    gSPEndDisplayList(gfx->poly_opa.d + 1);

    //initialize segment 09 to point to the custom display list
    gMoveWd(opa_ptr++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);

    //jump to the custom display list
    gSPDisplayList(opa_ptr++, dlist);
}
