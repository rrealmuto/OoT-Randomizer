#include "pots.h"
#include "n64.h"
#include "gfx.h"

#define BROWN_FRONT_TEXTURE 0x06001798
#define BROWN_BASE_TEXTURE 0x06002798
#define GOLD_FRONT_TEXTURE 0x06002F98
#define GOLD_BASE_TEXTURE 0x06003798

#define POT_BASE 1
#define POT_LID 3

uint32_t POT_TEXTURE_MATCH_CONTENTS = 0;

extern void* GILDED_POT_FRONT_TEXTURE;
extern void* GILDED_POT_BASE_TEXTURE;
extern void* SILVER_POT_FRONT_TEXTURE;
extern void* SILVER_POT_BASE_TEXTURE;
extern void* SKULL_POT_FRONT_TEXTURE;
extern void* SKULL_POT_BASE_TEXTURE;

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

            color = item_row->pot_type;
        }
    }

    ((uint8_t*)actor)[0x01EC] = size;
    ((uint8_t*)actor)[0x01ED] = color;
}

void draw_pot(z64_game_t* game, int part, void* unk, void* unk2,
    z64_actor_t *actor, Gfx **opa_ptr) {
    if (part != POT_BASE && part != POT_LID)
        return;

    z64_gfx_t *gfx = game->common.gfx;
    int pot_type = ((uint8_t*)actor)[0x01ED];

    //write matrix
    Mtx_t *mtx = write_matrix_stack_top(gfx);
    gSPMatrix((*opa_ptr)++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

    int dlist;

    if (part == POT_BASE) {
        if (pot_type == GOLD_POT)
            dlist = 0x06000AE8;
        else
            dlist = 0x060006F0;

    }
    else { //(part == POT_LID)
        if (pot_type == GOLD_POT)
            dlist = 0x06001678;
        else
            dlist = 0x060010C0;
    }

    if (pot_type != GOLD_POT) {
        //set texture type
        void* frontTexture = (void*)BROWN_FRONT_TEXTURE;
        void* baseTexture = (void*)BROWN_BASE_TEXTURE;

        if (POT_TEXTURE_MATCH_CONTENTS) {
            if (pot_type == GILDED_POT) {
                frontTexture = &GILDED_POT_FRONT_TEXTURE;
                baseTexture = &GILDED_POT_BASE_TEXTURE;
            }
            else if (pot_type == SILVER_POT) {
                frontTexture = &SILVER_POT_FRONT_TEXTURE;
                baseTexture = &SILVER_POT_BASE_TEXTURE;
            }
            else if (pot_type == SKULL_POT_SMALL || pot_type == SKULL_POT_BIG) {
                frontTexture = &SKULL_POT_FRONT_TEXTURE;
                baseTexture = &SKULL_POT_BASE_TEXTURE;
            }
        }

        //the brown pot's base and lid dlist has been modified to jump to
        //segment 09 in order to dynamically set the pot front and base textures
        gfx->poly_opa.d -= 4;
        gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, frontTexture);
        gSPEndDisplayList(gfx->poly_opa.d + 1);
        gDPSetTextureImage(gfx->poly_opa.d + 2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, baseTexture);
        gSPEndDisplayList(gfx->poly_opa.d + 3);

        gMoveWd((*opa_ptr)++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);
    }
    gSPDisplayList((*opa_ptr)++, dlist);
}
