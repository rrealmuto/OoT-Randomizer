#include "silver_rupee_info.h"
#include "dungeon_info.h"
#include "item_effects.h"
#include "gfx.h"
#include "text.h"
#include "util.h"
#include "save.h"
#include <string.h>

extern uint32_t CFG_DUNGEON_INFO_ENABLE;
extern unsigned char CFG_DUNGEON_IS_MQ[14];
extern silver_rupee_data_t silver_rupee_vars[0x16][2];
extern extended_savecontext_static_t extended_savectx;

silver_rupee_info_t silver_rupee_info[] = {
    {DODONGO_ID, "DC Staircase"},
    {ICE_ID, "Ice Scythe"},
    {ICE_ID, "Ice Block Push"},
    {BOTW_ID, "BotW Basement"},
    {SHADOW_ID, "Shdw Scythe"},
    {SHADOW_ID, "Shdw Invis Blades"},
    {SHADOW_ID, "Shdw Pit"},
    {SHADOW_ID, "Shdw Invis Spikes"},
    {GTG_ID, "GTG Slopes Room"},
    {GTG_ID, "GTG Lava Room"},
    {GTG_ID, "GTG Water Room"},
    {SPIRIT_ID, "Sprt Child Torches"},
    {SPIRIT_ID, "Sprt Adult Boulders"},
    {SPIRIT_ID, "Sprt Lobby"},
    {SPIRIT_ID, "Sprt Sun Block"},
    {SPIRIT_ID, "Sprt Adult Climb"},
    {CASTLE_ID, "Spirit Trial"},
    {CASTLE_ID, "Light Trial"},
    {CASTLE_ID, "Fire Trial"},
    {CASTLE_ID, "Shadow Trial"},
    {CASTLE_ID, "Water Trial"},
    {CASTLE_ID, "Forest Trial"}
};

void draw_silver_rupee_info(z64_disp_buf_t *db)
{
    int draw = CFG_DUNGEON_INFO_ENABLE &&
        z64_game.pause_ctxt.state == 6 &&
        z64_game.pause_ctxt.screen_idx == 0 &&
        !z64_game.pause_ctxt.changing &&
        z64_ctxt.input[0].raw.pad.dd;
    if (!draw) {
        return;
    }

    db->p = db->buf;

    // Call setup display list
    gSPDisplayList(db->p++, &setup_db);

    // Set up dimensions

    int icon_size = 16;
    int padding = 1;
    int rows = 13;

    int bg_width =
        (35 * font_sprite.tile_w) +
        (8 * padding);
    int bg_height = (rows * icon_size) + ((rows + 1) * padding);
    int bg_left = (Z64_SCREEN_WIDTH - bg_width) / 2;
    int bg_top = (Z64_SCREEN_HEIGHT - bg_height) / 2;

    int left = bg_left + padding;
    int start_top = bg_top + padding;

    // Draw background

    gDPSetCombineMode(db->p++, G_CC_PRIMITIVE, G_CC_PRIMITIVE);
    gDPSetPrimColor(db->p++, 0, 0, 0x00, 0x00, 0x00, 0xD0);
    gSPTextureRectangle(db->p++,
            bg_left<<2, bg_top<<2,
            (bg_left + bg_width)<<2, (bg_top + bg_height)<<2,
            0,
            0, 0,
            1<<10, 1<<10);

    gDPPipeSync(db->p++);
    gDPSetCombineMode(db->p++, G_CC_MODULATEIA_PRIM, G_CC_MODULATEIA_PRIM);

    
    gDPSetPrimColor(db->p++, 0, 0, 0xFF, 0xFF, 0xFF, 0xFF);

    uint8_t draw_count = 0;
    // Draw names
    int max_name_length = 0;
    for(int i = 0; i < 22; i++)
    {
        silver_rupee_info_t info = silver_rupee_info[i];
        silver_rupee_data_t var = silver_rupee_vars[i][CFG_DUNGEON_IS_MQ[info.dungeon_id]];
        uint8_t count = extended_savectx.silver_rupee_counts[i];
        if(var.needed_count != 0xFF && count > 0)
        {
            char count_str[4] = " 0\0";
            if(count >= 10)
            {
                count_str[1] += count / 10;
                count_str[2] = count % 10;
            }
            else
            {
                count_str[1] += count;
            }
            int name_length = text_len(info.puzzle_name);
            
            int top = start_top + ((icon_size + padding) * (draw_count % 13)) + 1;
            int text_left = draw_count < 13 ? left : left + max_name_length * font_sprite.tile_w;
            text_print(info.puzzle_name, text_left, top);
            text_print(count_str, text_left + name_length * font_sprite.tile_w, top);
            name_length += text_len(count_str);
            
            if(name_length > max_name_length && i < 13)
            {
                max_name_length = name_length;
            }
            
            draw_count++;
        }
    }

    // Finish

    text_flush(db);

    gDPFullSync(db->p++);
    gSPEndDisplayList(db->p++);
}