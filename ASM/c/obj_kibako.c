#include "item_table.h"
#include "get_items.h"
#include "z64.h"
#include "textures.h"

#define SMALLCRATE_DLIST (z64_gfx_t *)0x05005290
#define SMALLCRATE_TEXTURE (uint8_t *)0x05011CA0
extern uint8_t POTCRATE_TEXTURES_MATCH_CONTENTS;

override_t get_smallcrate_override(z64_actor_t *actor, z64_game_t *game) {
    // make a dummy EnItem00 with enough info to get the override

    uint8_t item = (actor->variable & 0x3F);
    if (item == 0x3F) {
        return (override_t){ 0 };
    }

    EnItem00 dummy;
    dummy.collectibleFlag = (actor->variable & 0x3F00) >> 8;
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;
    dummy.actor.variable = item;

    if (!should_override_collectible(&dummy)) {
        return (override_t){ 0 };
    }

    return lookup_override((z64_actor_t *)&dummy, game->scene_index, 0);
}

void ObjKibako_Draw(z64_actor_t *actor, z64_game_t *game) {
    uint8_t *texture = SMALLCRATE_TEXTURE; // get original texture

    override_t crate_override = get_smallcrate_override(actor, game);
    if (POTCRATE_TEXTURES_MATCH_CONTENTS == PTMC_UNCHECKED && crate_override.key.all != 0) {
        texture = get_texture(TEXTURE_ID_SMALLCRATE_GOLD);
    } else if (POTCRATE_TEXTURES_MATCH_CONTENTS == PTMC_CONTENTS && crate_override.key.all != 0) {
        uint16_t item_id = resolve_upgrades(crate_override.value.item_id);
        item_row_t *row = get_item_row(crate_override.value.looks_like_item_id);
        if (row == NULL) {
            row = get_item_row(crate_override.value.item_id);
        }
        if (row->chest_type == GILDED_CHEST) {
            texture = get_texture(TEXTURE_ID_SMALLCRATE_GOLD);
        } else if (row->chest_type == SILVER_CHEST) {
            texture = get_texture(TEXTURE_ID_SMALLCRATE_KEY);
        } else if (row->chest_type == GOLD_CHEST) {
            texture = get_texture(TEXTURE_ID_SMALLCRATE_BOSSKEY);
        } else if (row->chest_type == SKULL_CHEST_SMALL || row->chest_type == SKULL_CHEST_BIG) {
            texture = get_texture(TEXTURE_ID_SMALLCRATE_SKULL);
        }
    }

    // push custom dlists (that set the palette and textures) to segment 09
    z64_gfx_t *gfx = game->common.gfx;
    gfx->poly_opa.d -= 2;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gSPEndDisplayList(gfx->poly_opa.d + 1);

    gMoveWd(gfx->poly_opa.p++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);

    // draw the original dlist that has been hacked in ASM to jump to the custom dlists
    z64_Gfx_DrawDListOpa(game, SMALLCRATE_DLIST);
}
