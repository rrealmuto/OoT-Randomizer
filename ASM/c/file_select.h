#ifndef FILE_SELECT_H
#define FILE_SELECT_H

#include "z64.h"
#define PASSWORD_LENGTH 6

void draw_file_select_hash(uint32_t fade_out_alpha, z64_menudata_t* menu_data);
uint8_t is_buffer_password_clear();

void Audio_StopCurrentMusic(uint16_t arg0);

#endif
