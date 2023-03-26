#include "item_draw_functions.h"

#include "z64.h"
#include "item_draw_table.h"
#include "sys_matrix.h"

typedef Gfx *(*append_setup_dl_fn)(Gfx *gfx, uint32_t dl_index);
typedef void (*append_setup_dl_26_to_opa_fn)(z64_gfx_t *gfx);
typedef void (*append_setup_dl_25_to_opa_fn)(z64_gfx_t *gfx);
typedef void (*append_setup_dl_25_to_xlu_fn)(z64_gfx_t *gfx);
typedef Gfx *(*gen_double_tile_fn)(z64_gfx_t *gfx, int32_t tile1, uint32_t x1, uint32_t y1, int32_t width1, int32_t height1,
                                int32_t tile2, uint32_t x2, uint32_t y2, int32_t width2, int32_t height2);

#define append_setup_dl ((append_setup_dl_fn)0x8007DFBC)
#define append_setup_dl_26_to_opa ((append_setup_dl_26_to_opa_fn)0x8007E1DC)
#define append_setup_dl_25_to_opa ((append_setup_dl_25_to_opa_fn)0x8007E298)
#define append_setup_dl_25_to_xlu ((append_setup_dl_25_to_xlu_fn)0x8007E2C0)
#define gen_double_tile ((gen_double_tile_fn)0x8007EB84)

void draw_gi_bombchu_and_masks(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_26_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_eggs_and_medallions(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_26_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[1].dlist);
}

void draw_gi_sold_out(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    gfx->poly_xlu.p = append_setup_dl(gfx->poly_xlu.p, 5);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_compass(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    gfx->poly_xlu.p = append_setup_dl(gfx->poly_xlu.p, 5);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
}

void draw_gi_various_opa0(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_various_opa1023(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[2].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[3].dlist);
}

void draw_gi_wallets(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[2].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[3].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[4].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[5].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[6].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[7].dlist);
}

void draw_gi_various_xlu01(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[0].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
}

void draw_gi_various_opa0_xlu1(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
}

void draw_rutos_letter(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    // Turn the model sideways
    rotate_Z_sys_matrix(1.57f, 1);

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
}

void draw_gi_coins_and_cuccos(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
}

void draw_gi_magic_arrows(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
}

void draw_gi_various_opa10_xlu32(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[3].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
}


void draw_gi_bullet_bags(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[3].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[4].dlist);
}

void draw_gi_small_rupees(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    scale_sys_matrix(0.7f, 0.7f, 0.7f, 1);

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[3].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
}

void draw_gi_goron_swords(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPSegment(gfx->poly_opa.p++, 0x08,
               gen_double_tile(gfx,
                               0, game->common.state_frames, 0, 32, 32,
                               1, 0, 0, 32, 32));
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_deku_nut(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPSegment(gfx->poly_opa.p++, 0x08,
               gen_double_tile(gfx,
                               0, game->common.state_frames * 6, game->common.state_frames * 6, 32, 32,
                               1, game->common.state_frames * 6, game->common.state_frames * 6, 32, 32));
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_recovery_heart(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_xlu(gfx);
    gSPSegment(gfx->poly_xlu.p++, 0x08,
               gen_double_tile(gfx,
                               0, 0, -(game->common.state_frames * 3), 32, 32,
                               1, 0, -(game->common.state_frames * 2), 32, 32));
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_fish_bottle(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_xlu(gfx);
    gSPSegment(gfx->poly_xlu.p++, 0x08,
               gen_double_tile(gfx,
                               0, 0, game->common.state_frames, 32, 32,
                               1, 0, game->common.state_frames, 32, 32));
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_magic_spells(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_xlu(gfx);
    gSPSegment(gfx->poly_xlu.p++, 0x08,
               gen_double_tile(gfx,
                               0, game->common.state_frames * 2, -(game->common.state_frames * 6), 32, 32,
                               1, game->common.state_frames, -(game->common.state_frames * 2), 32, 32));
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[0].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
}

void draw_gi_scales(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_xlu(gfx);
    gSPSegment(gfx->poly_xlu.p++, 0x08,
               gen_double_tile(gfx,
                               0, game->common.state_frames * 2, -(game->common.state_frames * 2), 64, 64,
                               1, game->common.state_frames * 4, -(game->common.state_frames * 4), 32, 32));
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[3].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_potions(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPSegment(gfx->poly_opa.p++, 0x08,
               gen_double_tile(gfx,
                               0, -game->common.state_frames, game->common.state_frames, 32, 32,
                               1, -game->common.state_frames, game->common.state_frames, 32, 32));
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[1].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[2].dlist);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[3].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[4].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[5].dlist);
}

void draw_gi_mirror_shield(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPSegment(gfx->poly_opa.p++, 0x08,
               gen_double_tile(gfx,
                               0, 0, (game->common.state_frames * 2) % 256, 64, 64,
                               1, 0, game->common.state_frames % 128, 32, 32));
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
}

void draw_gi_gs_token(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPSegment(gfx->poly_xlu.p++, 0x08,
               gen_double_tile(gfx,
                               0, 0, -(game->common.state_frames * 5), 32, 32,
                               1, 0, 0, 32, 64));
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
}

void draw_gi_blue_fire_candle(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPSegment(gfx->poly_xlu.p++, 0x08,
               gen_double_tile(gfx,
                               0, 0, 0, 16, 32,
                               1, game->common.state_frames * 1, -(game->common.state_frames * 8), 16, 32));
    duplicate_sys_matrix();
    translate_sys_matrix(-8.0f, -2.0f, 0.0f, 1);
    update_sys_matrix(game->mf_11DA0);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
    pop_sys_matrix();
}

void draw_gi_fairy_lantern(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
    gSPSegment(gfx->poly_xlu.p++, 0x08,
               gen_double_tile(gfx,
                               0, 0, 0, 32, 32,
                               1, game->common.state_frames, -(game->common.state_frames * 6), 32, 32));
    duplicate_sys_matrix();
    update_sys_matrix(game->mf_11DA0);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
    pop_sys_matrix();
}

void draw_gi_poe_bottles(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
    gSPSegment(gfx->poly_xlu.p++, 0x08,
               gen_double_tile(gfx,
                               0, 0, 0, 16, 32,
                               1, game->common.state_frames * 1, -(game->common.state_frames * 6), 16, 32));
    duplicate_sys_matrix();
    update_sys_matrix(game->mf_11DA0);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[3].dlist);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[2].dlist);
    pop_sys_matrix();
}

void draw_gi_song_notes(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;
    colorRGBA8_t env_color = item_draw_table[draw_id].args[1].color;

    if (item_draw_table[draw_id].args[2].dlist) {
        scale_sys_matrix(1.5f, 1.5f, 1.5f, 1);
    }

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetEnvColor(gfx->poly_xlu.p++, env_color.r, env_color.g, env_color.b, env_color.a);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_small_keys(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;
    colorRGBA8_t prim_color = item_draw_table[draw_id].args[1].color;
    colorRGBA8_t env_color = item_draw_table[draw_id].args[2].color;

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetCombineMode(gfx->poly_opa.p++, G_CC_MODULATEI_PRIM, G_CC_MODULATEI_PRIM);
    gDPSetPrimColor(gfx->poly_opa.p++, 0, 0x80, prim_color.r, prim_color.g, prim_color.b, prim_color.a);
    gDPSetEnvColor(gfx->poly_opa.p++, env_color.r, env_color.g, env_color.b, env_color.a);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_boss_keys(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;
    colorRGBA8_t prim_color = item_draw_table[draw_id].args[2].color;
    colorRGBA8_t env_color = item_draw_table[draw_id].args[3].color;

    colorRGBA8_t prim_color_key = item_draw_table[draw_id].args[4].color;
    colorRGBA8_t env_color_key = item_draw_table[draw_id].args[5].color;
    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetPrimColor(gfx->poly_opa.p++, 0, 0x80, prim_color_key.r, prim_color_key.g, prim_color_key.b, prim_color_key.a);
    gDPSetEnvColor(gfx->poly_opa.p++, env_color_key.r, env_color_key.g, env_color_key.b, env_color_key.a);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetPrimColor(gfx->poly_xlu.p++, 0, 0x80, prim_color.r, prim_color.g, prim_color.b, prim_color.a);
    gDPSetEnvColor(gfx->poly_xlu.p++, env_color.r, env_color.g, env_color.b, env_color.a);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[1].dlist);
}

extern uint8_t CFG_CORRECT_MODEL_COLORS;
extern colorRGB16_t CFG_A_BUTTON_COLOR;
void draw_gi_a_button(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;
    colorRGBA8_t prim_color = item_draw_table[draw_id].args[1].color;
    if (CFG_CORRECT_MODEL_COLORS) {
        prim_color.r = CFG_A_BUTTON_COLOR.r;
        prim_color.g = CFG_A_BUTTON_COLOR.g;
        prim_color.b = CFG_A_BUTTON_COLOR.b;
    }

    append_setup_dl_25_to_xlu(gfx);
    gSPMatrix(gfx->poly_xlu.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetPrimColor(gfx->poly_xlu.p++, 0, 0x80, prim_color.r, prim_color.g, prim_color.b, prim_color.a);
    gSPDisplayList(gfx->poly_xlu.p++, item_draw_table[draw_id].args[0].dlist);
}

extern colorRGB16_t CFG_C_BUTTON_COLOR;
void draw_gi_c_button_vertical(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;
    colorRGBA8_t prim_color = item_draw_table[draw_id].args[1].color;
    if (CFG_CORRECT_MODEL_COLORS) {
        prim_color.r = CFG_C_BUTTON_COLOR.r;
        prim_color.g = CFG_C_BUTTON_COLOR.g;
        prim_color.b = CFG_C_BUTTON_COLOR.b;
    }

    rotate_Z_sys_matrix(item_draw_table[draw_id].args[2].dlist*3.14f, 1);

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetPrimColor(gfx->poly_opa.p++, 0, 0x80, prim_color.r, prim_color.g, prim_color.b, prim_color.a);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
}

void draw_gi_c_button_horizontal(z64_game_t *game, uint32_t draw_id) {
    z64_gfx_t *gfx = game->common.gfx;
    colorRGBA8_t prim_color = item_draw_table[draw_id].args[1].color;
    if (CFG_CORRECT_MODEL_COLORS) {
        prim_color.r = CFG_C_BUTTON_COLOR.r;
        prim_color.g = CFG_C_BUTTON_COLOR.g;
        prim_color.b = CFG_C_BUTTON_COLOR.b;
    }

    rotate_Z_sys_matrix(item_draw_table[draw_id].args[2].dlist*3.14f, 1);

    append_setup_dl_25_to_opa(gfx);
    gSPMatrix(gfx->poly_opa.p++, append_sys_matrix(gfx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetPrimColor(gfx->poly_opa.p++, 0, 0x80, prim_color.r, prim_color.g, prim_color.b, prim_color.a);
    gSPDisplayList(gfx->poly_opa.p++, item_draw_table[draw_id].args[0].dlist);
}
