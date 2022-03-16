#ifndef TEXTURES_H
#define TEXTURES_H

#include "z64.h"
#include "util.h"

#define TEXTURE_ID_NONE             0
#define TEXTURE_ID_POT_GOLD         1
#define TEXTURE_ID_POT_KEY          2
#define TEXTURE_ID_POT_BOSSKEY      3
#define TEXTURE_ID_POT_SKULL        4

#define TEXTURE_ID_CRATE_DEFAULT    5
#define TEXTURE_ID_CRATE_GOLD       6
#define TEXTURE_ID_CRATE_KEY        7
#define TEXTURE_ID_CRATE_SKULL      8
#define TEXTURE_ID_CRATE_BOSSKEY      9

uint8_t* get_texture(uint16_t textureID);
void init_textures();

#endif