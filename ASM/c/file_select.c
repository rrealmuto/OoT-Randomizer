#include "file_select.h"

#include "audio.h"
#include "file_icons.h"
#include "file_message.h"
#include "gfx.h"
#include "music.h"
#include "text.h"
#include "util.h"
#include "save.h"
#include "debug.h"

sprite_t* hash_sprites[2] = {
    &items_sprite,
    &quest_items_sprite,
};

typedef struct {
    uint8_t sprite_index;
    uint8_t tile_index;
} hash_symbol_t;

hash_symbol_t hash_symbols[32] = {
    { 0,  0 }, // Deku Stick
    { 0,  1 }, // Deku Nut
    { 0,  3 }, // Bow
    { 0,  6 }, // Slingshot
    { 0,  7 }, // Fairy Ocarina
    { 0,  9 }, // Bombchu
    { 0, 11 }, // Longshot
    { 0, 14 }, // Boomerang
    { 0, 15 }, // Lens of Truth
    { 0, 16 }, // Beans
    { 0, 17 }, // Megaton Hammer
    { 0, 25 }, // Bottled Fish
    { 0, 26 }, // Bottled Milk
    { 0, 43 }, // Mask of Truth
    { 0, 44 }, // SOLD OUT
    { 0, 46 }, // Cucco
    { 0, 48 }, // Mushroom
    { 0, 50 }, // Saw
    { 0, 53 }, // Frog
    { 0, 60 }, // Master Sword
    { 0, 64 }, // Mirror Shield
    { 0, 65 }, // Kokiri Tunic
    { 0, 70 }, // Hover Boots
    { 0, 81 }, // Silver Gauntlets
    { 0, 84 }, // Gold Scale
    { 1,  9 }, // Stone of Agony
    { 1, 11 }, // Skull Token
    { 1, 12 }, // Heart Container
    { 1, 14 }, // Boss Key
    { 1, 15 }, // Compass
    { 1, 16 }, // Map
    { 1, 19 }, // Big Magic
};

extern uint8_t CFG_FILE_SELECT_HASH[5];

uint8_t password_state = 0;
#define TEXT_WIDTH 8
#define TEXT_HEIGHT 9
#define BUTTON_WIDTH 12
#define BUTTON_HEIGHT 12
extern uint8_t PASSWORD[PASSWORD_LENGTH];
uint8_t buffer_password[PASSWORD_LENGTH] = {-1, -1, -1, -1, -1, -1,};

uint8_t is_saved_password_clear(z64_menudata_t* menu_data) {
    // Player can save in file 1 and use file 2 later, so we look at both of them.
    uint8_t fileOkPassword[2] = {1, 1};
    for (uint8_t slotFile = 0; slotFile < 2; slotFile++) {
        z64_file_t* file = &menu_data->sram_buffer->primary_saves[slotFile].original_save;
        extended_savecontext_static_t* extended = &(((extended_sram_file_t*)file)->additional_save_data.extended);
        for (uint8_t i = 0 ; i < PASSWORD_LENGTH; i++) {
            if (extended->password[i] != PASSWORD[i]) {
                fileOkPassword[slotFile] = 0;
            }
        }
    }
    return (fileOkPassword[0] || fileOkPassword[1]) ? 1 : 0;
}

uint8_t is_buffer_password_clear() {
    for (uint8_t i = 0 ; i < PASSWORD_LENGTH; i++) {
        if (buffer_password[i] != PASSWORD[i]) {
            return 0;
        }
    }
    return 1;
}

void reset_buffer() {
    for (uint8_t i = 0 ; i < PASSWORD_LENGTH; i++) {
        buffer_password[i] = -1;
    }
}

void manage_password(z64_disp_buf_t* db, z64_menudata_t* menu_data) {

    if (is_saved_password_clear(menu_data) == 0 && is_buffer_password_clear() == 0) { // Draw "Password locked" at the place of the Controls texture.
        int left = 90;
        int top = 204;
        int padding = 8;
        sprite_load(db, &quest_items_sprite, 17, 1);

        gDPPipeSync(db->p++);
        gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
        gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
        sprite_draw(db, &quest_items_sprite, 0, left, top - 2, BUTTON_WIDTH, BUTTON_HEIGHT);
        text_print_size("Password locked", left + TEXT_HEIGHT + padding, top, TEXT_WIDTH);
        sprite_draw(db, &quest_items_sprite, 0, left + TEXT_WIDTH + 2*padding + 15*font_sprite.tile_w, top - 2, BUTTON_WIDTH, BUTTON_HEIGHT);
        text_flush_size(db, TEXT_WIDTH, TEXT_HEIGHT, 0, 0);
    }

    if (menu_data->menu_transition > 0) { // In the File 1/File 2 menu.

        if (password_state > 0) {
            uint8_t left_password = 0x37;
            uint8_t top_password = 0x5C;

            gDPPipeSync(db->p++);
            gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
            gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);
            text_print_size("Enter Password", left_password, top_password, TEXT_WIDTH);
            text_flush_size(db, TEXT_WIDTH, TEXT_HEIGHT, 0, 0);

            if (is_buffer_password_clear()) {
                password_state = 0;
                z64_PlaySFXID(NA_SE_SY_CORRECT_CHIME);
            }
            else {
                if (z64_game.common.input[0].pad_pressed.b || password_state > 6) {
                    z64_PlaySFXID(NA_SE_SY_ERROR);
                    password_state = 0;
                    reset_buffer();
                }
                else {
                    sprite_load(db, &ocarina_button_sprite, 0, 5);
                    if (z64_game.common.input[0].pad_pressed.a) {
                        buffer_password[password_state - 1] = 0;
                        password_state++;
                    }
                    if (z64_game.common.input[0].pad_pressed.cd) {
                        buffer_password[password_state - 1] = 1;
                        password_state++;
                    }
                    if (z64_game.common.input[0].pad_pressed.cr) {
                        buffer_password[password_state - 1] = 2;
                        password_state++;
                    }
                    if (z64_game.common.input[0].pad_pressed.cl) {
                        buffer_password[password_state - 1] = 3;
                        password_state++;
                    }
                    if (z64_game.common.input[0].pad_pressed.cu) {
                        buffer_password[password_state - 1] = 4;
                        password_state++;
                    }
                    for (uint8_t i = 0 ; i < PASSWORD_LENGTH; i++) {
                        gDPSetPrimColor(db->p++, 0, 0, 0xF4, 0xEC, 0x30, 0xFF); // Yellow C buttons
                        if (buffer_password[i] == 0) { // A is blue
                            gDPSetPrimColor(db->p++, 0, 0, 0x00, 0x00, 0xFF, 0xFF);
                        }
                        sprite_draw(db, &ocarina_button_sprite, buffer_password[i],
                            left_password + i * (BUTTON_WIDTH + 5), top_password + 0x0C, BUTTON_WIDTH, BUTTON_HEIGHT);
                    }
                }
            }
        }

        if (menu_data->selected_sub_item == 0) { // On the Ok option.
            if (z64_game.common.input[0].pad_pressed.a || z64_game.common.input[0].pad_pressed.s) {
                if (is_saved_password_clear(menu_data) || is_buffer_password_clear()) { // Vanilla behaviour.
                    z64_PlaySFXID(NA_SE_SY_FSEL_DECIDE_L);
                    menu_data->menu_transition = 6; // SM_FADE_OUT
                    Audio_StopCurrentMusic(0xF);
                }
                else {
                    if (password_state == 0) {
                        password_state++;
                    }
                }
            }
        }
    }
}

void draw_file_select_hash(uint32_t fade_out_alpha, z64_menudata_t* menu_data) {
    z64_disp_buf_t* db = &(z64_ctxt.gfx->poly_opa);

    // Call setup display list
    gSPDisplayList(db->p++, &setup_db);

    int icon_count = 5;
    int icon_size = 24;
    int padding = 8;
    int width = (icon_count * icon_size) + ((icon_count - 1) * padding);
    int left = (Z64_SCREEN_WIDTH - width) / 2;
    int top = 12;

    gDPPipeSync(db->p++);
    gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);

    for (int i = 0; i < icon_count; i++) {
        uint8_t sym_index = CFG_FILE_SELECT_HASH[i];
        hash_symbol_t* sym_desc = &(hash_symbols[sym_index]);
        sprite_t* sym_sprite = hash_sprites[sym_desc->sprite_index];

        sprite_load(db, sym_sprite, sym_desc->tile_index, 1);
        sprite_draw(db, sym_sprite, 0, left, top, icon_size, icon_size);

        left += icon_size + padding;
    }

    draw_file_message(db, menu_data);
    if (password_state == 0) {
        draw_file_icons(db, menu_data);
    }
    display_song_name_on_file_select(db);

    manage_password(db, menu_data);
    // Fade out once a file is selected

    gDPPipeSync(db->p++);
    gDPSetCombineMode(db->p++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(db->p++, 0, 0, 0x00, 0x00, 0x00, fade_out_alpha);
    gSPTextureRectangle(db->p++,
            0, 0,
            Z64_SCREEN_WIDTH<<2, Z64_SCREEN_HEIGHT<<2,
            0,
            0, 0,
            1<<10, 1<<10);
}
