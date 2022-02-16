#include "pots.h"
#include "n64.h"
#include "gfx.h"

#define BROWN_FRONT_TEXTURE 0x06000000

uint32_t POT_TEXTURE_MATCH_CONTENTS = 0;

extern void* GILDED_CHEST_FRONT_TEXTURE;
extern void* SILVER_CHEST_FRONT_TEXTURE;
extern void* SKULL_CHEST_FRONT_TEXTURE;

extern Mtx_t* write_matrix_stack_top(z64_gfx_t* gfx);
asm(".equ write_matrix_stack_top, 0x800AB900");

void get_pot_override(z64_actor_t *actor) {
    uint8_t size  = ((uint8_t*)actor)[0x01E9];
    uint8_t color = size;

    if (POT_TEXTURE_MATCH_CONTENTS) {
        uint8_t scene = z64_game.scene_index;
        uint8_t item_id = (actor->variable & 0x0FE0) >> 5;

        override_t override = lookup_override(actor, scene, item_id);
        if (override.value.item_id != 0) {
            item_row_t *item_row = get_item_row(override.value.looks_like_item_id);
            if (item_row == NULL) {
                item_row = get_item_row(override.value.item_id);
            }

            color = item_row->chest_type;
        }
    }

    ((uint8_t*)actor)[0x01EC] = size;
    ((uint8_t*)actor)[0x01ED] = color;
}

void draw_pot(z64_actor_t *actor, z64_game_t* game) {
    z64_gfx_t *gfx = game->common.gfx;
    int chest_type = ((uint8_t*)actor)[0x01ED];

    //write matrix
    Mtx_t *mtx = write_matrix_stack_top(gfx);
    gSPMatrix((*opa_ptr)++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD);

    if (chest_type != GOLD_CHEST) {
        //set texture type
        void* frontTexture = (void*)BROWN_FRONT_TEXTURE;

        if (POT_TEXTURE_MATCH_CONTENTS) {
            if (chest_type == GILDED_CHEST) {
                frontTexture = &GILDED_CHEST_FRONT_TEXTURE;
            }
            else if (chest_type == SILVER_CHEST) {
                frontTexture = &SILVER_CHEST_FRONT_TEXTURE;
            }
            else if (chest_type == SKULL_CHEST_SMALL || chest_type == SKULL_CHEST_BIG) {
                frontTexture = &SKULL_CHEST_FRONT_TEXTURE;
            }
        }

        //the brown pot's front dlist has been modified to jump to
        //segment 09 in order to dynamically set the pot front texture
        gfx->poly_opa.d -= 2;
        gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, frontTexture);
        gSPEndDisplayList(gfx->poly_opa.d + 1);

        gMoveWd((*opa_ptr)++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);
    }
    gSPDisplayList((*opa_ptr)++, 0x060017C0); //object_tsubo_DL_0017C0
}
