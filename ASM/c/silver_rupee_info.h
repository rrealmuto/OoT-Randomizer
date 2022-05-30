#include "z64.h"
#include "item_effects.h"

#ifndef SILVER_RUPEE_INFO_H
#define SILVER_RUPEE_INFO_H


typedef struct {
    dungeon dungeon_id;
    char* puzzle_name;
} silver_rupee_info_t;


void draw_silver_rupee_info(z64_disp_buf_t *db);

#endif