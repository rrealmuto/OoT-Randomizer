#include "z64.h"
#include "gfx.h"
#include "text.h"
#include "enemy_spawn_shuffle.h"
#include "ovl_kaleidoscope.h"

#define KaleidoScope_DrawWorldMap   0x8081CE54
#define KaleidoScope_DrawDungeonMap 0x8081b660
#define PauseMapMark_Draw   0x808292e8

#define MAX_LINES 13

menu_ctx soul_menu_ctx = {0, NUM_ENEMY_SOULS, 0, 0, soul_menu_handler, SOUL_MENU_NAMES};
menu_ctx regional_soul_menu_ctx = {0, NUM_REGIONAL_ENEMY_SOULS, 0,0, soul_menu_handler, REGIONAL_SOUL_MENU_NAMES};

Vtx background_vertices[4];
int menu_count = 1;
MENU_PAGE menu_page = 0;
menu_ctx* menus[3]; // Increase this to add more menus. Index 0 is unused and corresponds to the vanilla map menu
uint8_t font_textures[NUM_FONT_CHARS * FONT_CHAR_TEX_WIDTH * FONT_CHAR_TEX_HEIGHT / 2] __attribute__ ((aligned (8)));
Gfx menu_dl_buffer[0x1000] __attribute__ ((aligned (16)));
Gfx* menu_dl_p __attribute__ ((aligned (16)));


void init_new_menus() {
    // Add menus here
    if(CFG_ENEMY_SPAWN_SHUFFLE == CFG_ENEMY_SPAWN_SHUFFLE_STANDARD) {
        menu_page = (MENU_PAGE)menu_count;
        menus[menu_count++] = &soul_menu_ctx;
    }
    else if(CFG_ENEMY_SPAWN_SHUFFLE == CFG_ENEMY_SPAWN_SHUFFLE_REGIONAL) {
        menu_page = (MENU_PAGE)menu_count;
        menus[menu_count++] = &regional_soul_menu_ctx;
    }
}

void draw_map_background(z64_game_t* globalCtx, z64_gfx_t* gfx, float x, float y, float w, float h)
{
    Vtx* v = &background_vertices[0];

    int xx[4];
    int yy[4];

    xx[0] = x;
    xx[1] = x + w;
    xx[2] = x + w;
    xx[3] = x;

    yy[0] = y;
    yy[1] = y;
    yy[2] = y - h;
    yy[3] = y - h;

    for (int i = 0; i < 4; ++i)
    {
        v[i].v.ob[0] = xx[i];
        v[i].v.ob[1] = yy[i];
        v[i].v.ob[2] = 0;
        v[i].v.tc[0] = 0;
        v[i].v.tc[1] = 0;
        v[i].v.flag = 0;
        v[i].v.cn[0] = 0x00;
        v[i].v.cn[1] = 0x00;
        v[i].v.cn[2] = 0x00;
        v[i].v.cn[3] = 0xff;
    }
    OPEN_DISPS(gfx);
    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 0, 0, 0, 255);
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPPipeSync(POLY_OPA_DISP++);
    gSPVertex(POLY_OPA_DISP++, (uint32_t)v & 0xffffffff, 4, 0);
    gSP2Triangles(
        POLY_OPA_DISP++,
        0, 2, 1, 0,
        0, 3, 2, 0
    );
    CLOSE_DISPS();
}

void soul_menu_handler(menu_ctx* menu, z64_game_t* globalCtx, z64_gfx_t* gfx) {
    update_soul_menu(menu, globalCtx);
    draw_soul_menu(menu, globalCtx, gfx);
}

void update_soul_menu(menu_ctx* menu, z64_game_t* globalCtx) {
    soul_menu_info* names = (soul_menu_info*)menu->extra;

    // Pressing A on a soul in the menu will toggle the soul on/off
    if(globalCtx->common.input->pad_pressed.a) {
        uint8_t soul_index = names[menu->curr_line].soul_id;
        if(flags_getsoul(soul_index)) { // Make sure we have the soul
            toggle_soul_enabled(soul_index); // Toggle the soul
            z64_Audio_PlaySoundGeneral(NA_SE_SY_DECIDE, (void *)0x80104394, 4, (float *)0x801043A0, (float *)0x801043A0, (uint8_t *)0x801043A8); // Play the menu sound
        }
    }

    // Handle menu navigation
    if(menu->cursor_delay == 0) { // Use a few frames of delay so we don't move too fast
        // Down
        if(globalCtx->common.input[0].adjusted_y < -30) {
            menu->curr_line++;
            menu->cursor_delay = 2;
            if(menu->curr_line > menu->total_lines - 1)
            {
                menu->curr_line = menu->total_lines - 1;
            }
            else {
                z64_Audio_PlaySoundGeneral(NA_SE_SY_CURSOR, (void *)0x80104394, 4, (float *)0x801043A0, (float *)0x801043A0, (uint8_t *)0x801043A8);
            }
        }
        // Up
        else if(globalCtx->common.input[0].adjusted_y > 30) {
            menu->curr_line--;
            menu->cursor_delay = 2;
            if(menu->curr_line < 0)
            {
                menu->curr_line = 0;
            }
            else {
                z64_Audio_PlaySoundGeneral(NA_SE_SY_CURSOR, (void *)0x80104394, 4, (float *)0x801043A0, (float *)0x801043A0, (uint8_t *)0x801043A8);
            }
        }
    }
    else if(menu->cursor_delay > 0) {
        menu->cursor_delay--; // Decrement the delay timer
    }

    // Restrict to min/max
    if(menu->curr_line < menu->curr_min_line) {
        menu->curr_min_line = menu->curr_line;
    }
    if(menu->curr_line > menu->curr_min_line + MAX_LINES - 1) {
        menu->curr_min_line = menu->curr_line - MAX_LINES + 1;
    }
}

void draw_soul_menu(menu_ctx* menu, z64_game_t* globalCtx, z64_gfx_t* gfx) {
    // test, just draw a red triangle
    // 1) Render the menu to a frame buffer

    // Set up the pipeline to use the new buffer, save old buffer?

    OPEN_DISPS(gfx);

    gDPPipeSync(POLY_OPA_DISP++);
    gDPSetOtherMode(POLY_OPA_DISP++, G_AD_DISABLE | G_CD_DISABLE |
        G_CK_NONE | G_TC_FILT |
        G_TD_CLAMP | G_TP_NONE |
        G_TL_TILE | G_TT_NONE |
        G_PM_NPRIMITIVE | G_CYC_1CYCLE |
        G_TF_BILERP, // HI
        G_AC_NONE | G_ZS_PRIM |
        G_RM_XLU_SURF | G_RM_XLU_SURF2), // LO
    gDPSetCombineMode(POLY_OPA_DISP++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

    int y = 60;
    //gDPSetCombineLERP(POLY_OPA_DISP++, PRIMITIVE, PRIMITIVE, TEXEL0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, PRIMITIVE,
    //                  PRIMITIVE, TEXEL0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0);

    menu_dl_p = &menu_dl_buffer[0];

    soul_menu_info* names = (soul_menu_info*)menu->extra;

    gDPPipeSync(menu_dl_p++);
    for(int i = menu->curr_min_line; i < menu->curr_min_line + MAX_LINES; i++) {
        if(flags_getsoul(names[i].soul_id)) {
            if(get_soul_enabled(names[i].soul_id)) {
                gDPSetPrimColor(menu_dl_p++, 0,0,255,255,255,255);
            }
            else {
                gDPSetPrimColor(menu_dl_p++, 0,0,255,0,0,255);
            }
        }
        else {
            gDPSetPrimColor(menu_dl_p++, 0,0,85,85,85,255);
        }
        int x = print_string(globalCtx, names[i].name, 50, y, .5);
        if(menu->curr_line == i) {
            print_string(globalCtx, "*", x, y, .5);
        }
        y+= 10;
    }
    gSPEndDisplayList(menu_dl_p++);
    gSPDisplayList(POLY_OPA_DISP++, &menu_dl_buffer[0]);
    //gDPSetCycleType(POLY_OPA_DISP++, G_CYC_1CYCLE);
    //gDPSetColorImage(POLY_OPA_DISP++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, gfx->frame_buffer);
    CLOSE_DISPS();
}

void text_new_init() {
    // Load the font texture from nes_font_static
    // Stored at 0x00928000
    // We really only need the first 95 characters
    DmaMgr_RequestSync(&(font_textures[0]), 0x928000, NUM_FONT_CHARS * FONT_CHAR_TEX_WIDTH * FONT_CHAR_TEX_HEIGHT / 2);
    menu_dl_p = &menu_dl_buffer[0];
}

void print_char(z64_game_t* globalCtx, char c, int x, int y, float scale) {
    void* textureImage = &(font_textures[0]) + (c - ' ') * 16 * 16 / 2;
    int32_t sCharTexSize = (scale) * 16.0f;
    int32_t sCharTexScale = 1024.0f / (scale);

    gDPPipeSync(menu_dl_p++);

    //gDPSetTextureImage(POLY_OPA_DISP++, G_IM_FMT_I, G_IM_SIZ_4b, FONT_CHAR_TEX_WIDTH, textureImage);
    //gDPLoadTile(gfx++, G_TX_LOADTILE, 0, 0, (FONT_CHAR_TEX_WIDTH - 1) << 2, (FONT_CHAR_TEX_HEIGHT - 1) << 2);

    gDPLoadTextureBlock_4b(menu_dl_p++, textureImage, G_IM_FMT_I, FONT_CHAR_TEX_WIDTH, FONT_CHAR_TEX_HEIGHT, 0,
                           G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                           G_TX_NOLOD);

    gDPPipeSync(menu_dl_p++);
    gSPTextureRectangle(menu_dl_p++, x << 2, y << 2, (x + sCharTexSize) << 2, (y + sCharTexSize) << 2, G_TX_RENDERTILE, 0, 0,
                        sCharTexScale, sCharTexScale);
}

int print_string(z64_game_t* globalCtx, char* str, int x, int y, float scale) {

    while(*str != 0x00) {
        print_char(globalCtx, *str, x,y, scale);
        x += 8;
        str++;
    }
    return x;
}

void draw_new_menus(z64_game_t* globalCtx, z64_gfx_t* gfx) {
    OPEN_DISPS(gfx)
    draw_map_background(globalCtx, gfx, -110.f, 59.f, 217.f, 128.f);
    CLOSE_DISPS();

    if(globalCtx->pause_ctxt.state == 6 && globalCtx->pause_ctxt.changing == 0 && globalCtx->pause_ctxt.screen_idx == 1) {
        globalCtx->pause_ctxt.cursor_pos = 0;
        menus[menu_page]->handler(menus[menu_page],globalCtx, gfx);
        /*switch(menu_page) {
            case(MENU_PAGE_SOULS): {
                update_soul_menu(globalCtx, gfx);
                draw_soul_menu(globalCtx, gfx);
                break;
            }
            default: {
                break;
            }
        }*/
    }

}

void KaleidoScope_DrawNewMap(z64_game_t* globalCtx, z64_gfx_t* gfx, kaleido_base_handler handler) {
    if(globalCtx->common.input[0].pad_pressed.cu && globalCtx->pause_ctxt.screen_idx == 1 && globalCtx->pause_ctxt.state == 6 && globalCtx->pause_ctxt.changing == 0) {
        menu_page = (menu_page + 1) % menu_count;
    }
    if(menu_page) {
        draw_new_menus(globalCtx, gfx);
    }
    else
    {
        handler = KaleidoManager_GetRamAddr(handler);
        handler(globalCtx, gfx);
    }

}

void KaleidoScope_DrawWorldMap_Callhook(z64_game_t* globalCtx, z64_gfx_t* gfx) {
    KaleidoScope_DrawNewMap(globalCtx, gfx, KaleidoScope_DrawWorldMap);
}

void KaleidoScope_DrawDungeonMap_Callhook(z64_game_t* globalCtx, z64_gfx_t* gfx) {
    KaleidoScope_DrawNewMap(globalCtx, gfx, KaleidoScope_DrawDungeonMap);
}

void PauseMapMark_Draw_CallHook(z64_game_t* globalCtx) {
    pause_mapmark_draw_func handler = KaleidoManager_GetRamAddr(PauseMapMark_Draw);
    if(menu_page == 0)
        handler(globalCtx);
}
