#ifndef ITEM_DRAW_FUNCTIONS_H
#define ITEM_DRAW_FUNCTIONS_H

#include "z64.h"

extern uint8_t CUSTOM_KEY_MODELS;
extern uint8_t CFG_CORRECT_MODEL_COLORS;
extern colorRGB16_t CFG_A_BUTTON_COLOR;
extern colorRGB16_t CFG_C_BUTTON_COLOR;

void draw_gi_bombchu_and_masks(z64_game_t *game, uint32_t draw_id);
void draw_gi_eggs_and_medallions(z64_game_t *game, uint32_t draw_id);
void draw_gi_sold_out(z64_game_t *game, uint32_t draw_id);
void draw_gi_compass(z64_game_t *game, uint32_t draw_id);
void draw_gi_various_opa0(z64_game_t *game, uint32_t draw_id);
void draw_gi_various_opa1023(z64_game_t *game, uint32_t draw_id);
void draw_gi_wallets(z64_game_t *game, uint32_t draw_id);
void draw_gi_various_xlu01(z64_game_t *game, uint32_t draw_id);
void draw_gi_various_opa0_xlu1(z64_game_t *game, uint32_t draw_id);
void draw_gi_coins_and_cuccos(z64_game_t *game, uint32_t draw_id);
void draw_gi_magic_arrows(z64_game_t *game, uint32_t draw_id);
void draw_gi_various_opa10_xlu32(z64_game_t *game, uint32_t draw_id);
void draw_rutos_letter(z64_game_t *game, uint32_t draw_id);
void draw_gi_bullet_bags(z64_game_t *game, uint32_t draw_id);
void draw_gi_small_rupees(z64_game_t *game, uint32_t draw_id);
void draw_gi_goron_swords(z64_game_t *game, uint32_t draw_id);
void draw_gi_deku_nut(z64_game_t *game, uint32_t draw_id);
void draw_gi_recovery_heart(z64_game_t *game, uint32_t draw_id);
void draw_gi_fish_bottle(z64_game_t *game, uint32_t draw_id);
void draw_gi_magic_spells(z64_game_t *game, uint32_t draw_id);
void draw_gi_scales(z64_game_t *game, uint32_t draw_id);
void draw_gi_potions(z64_game_t *game, uint32_t draw_id);
void draw_gi_mirror_shield(z64_game_t *game, uint32_t draw_id);
void draw_gi_gs_token(z64_game_t *game, uint32_t draw_id);
void draw_gi_blue_fire_candle(z64_game_t *game, uint32_t draw_id);
void draw_gi_fairy_lantern(z64_game_t *game, uint32_t draw_id);
void draw_gi_poe_bottles(z64_game_t *game, uint32_t draw_id);
void draw_gi_song_notes(z64_game_t *game, uint32_t draw_id);
void draw_gi_chubag(z64_game_t *game, uint32_t draw_id);
void draw_gi_small_keys(z64_game_t *game, uint32_t draw_id);
void draw_gi_shrink_keys(z64_game_t *game, uint32_t draw_id);
void draw_gi_boss_keys(z64_game_t *game, uint32_t draw_id);
void draw_gi_boss_altered(z64_game_t *game, uint32_t draw_id);
void draw_gi_boss_altflip(z64_game_t *game, uint32_t draw_id);
void draw_gi_silver_rupee_pouch(z64_game_t *game, uint32_t draw_id);
void draw_gi_a_button(z64_game_t *game, uint32_t draw_id);
void draw_gi_c_button_vertical(z64_game_t *game, uint32_t draw_id);
void draw_gi_c_button_horizontal(z64_game_t *game, uint32_t draw_id);
void draw_gi_fairy(z64_game_t *game, uint32_t draw_id);
void draw_gi_nothing(z64_game_t *game, uint32_t draw_id);

#endif
