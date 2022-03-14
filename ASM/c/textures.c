#include "textures.h"



#define LEN_TEXTURE_TABLE 20

typedef struct {
    uint16_t textureID;
    file_t file;
} texture_t;

texture_t texture_table[LEN_TEXTURE_TABLE] = 
{
    [TEXTURE_ID_POT_GOLD] = { .textureID = TEXTURE_ID_POT_GOLD},
    [TEXTURE_ID_POT_KEY] = {.textureID = TEXTURE_ID_POT_KEY},
    [TEXTURE_ID_POT_BOSSKEY] = {.textureID = TEXTURE_ID_POT_BOSSKEY},
    [TEXTURE_ID_POT_SKULL] = {.textureID = TEXTURE_ID_POT_SKULL},
    [TEXTURE_ID_CRATE_TOP_DEFAULT] = {.textureID = TEXTURE_ID_CRATE_TOP_DEFAULT},
    [TEXTURE_ID_CRATE_TOP_GOLD] = {.textureID = TEXTURE_ID_CRATE_TOP_GOLD},
    [TEXTURE_ID_CRATE_TOP_KEY] = {.textureID = TEXTURE_ID_CRATE_TOP_KEY},
    [TEXTURE_ID_CRATE_TOP_BOSSKEY] = {.textureID = TEXTURE_ID_CRATE_TOP_BOSSKEY},
    [TEXTURE_ID_CRATE_TOP_SKULL] = {.textureID = TEXTURE_ID_CRATE_TOP_SKULL},
    [TEXTURE_ID_CRATE_SIDE_DEFAULT] = {.textureID = TEXTURE_ID_CRATE_SIDE_DEFAULT},
    [TEXTURE_ID_CRATE_SIDE_GOLD] = {.textureID = TEXTURE_ID_CRATE_SIDE_GOLD},
    [TEXTURE_ID_CRATE_SIDE_KEY] = {.textureID = TEXTURE_ID_CRATE_SIDE_KEY},
    [TEXTURE_ID_CRATE_SIDE_BOSSKEY] = {.textureID = TEXTURE_ID_CRATE_SIDE_BOSSKEY},
    [TEXTURE_ID_CRATE_SIDE_SKULL] = {.textureID = TEXTURE_ID_CRATE_SIDE_SKULL},
    [TEXTURE_ID_CRATE_PALETTE_DEFAULT] = {.textureID = TEXTURE_ID_CRATE_PALETTE_DEFAULT},
    [TEXTURE_ID_CRATE_PALETTE_GOLD] = {.textureID = TEXTURE_ID_CRATE_PALETTE_GOLD},
    [TEXTURE_ID_CRATE_PALETTE_KEY] = {.textureID = TEXTURE_ID_CRATE_PALETTE_KEY},
    [TEXTURE_ID_CRATE_PALETTE_BOSSKEY] = {.textureID = TEXTURE_ID_CRATE_PALETTE_BOSSKEY},
    [TEXTURE_ID_CRATE_PALETTE_SKULL] = {.textureID = TEXTURE_ID_CRATE_PALETTE_SKULL},
};

uint8_t* get_texture(uint16_t textureID)
{
    return texture_table[textureID].file.buf;
}

void init_textures()
{
    for(int i = 0; i < LEN_TEXTURE_TABLE; i++)
    {
        if(texture_table[i].file.vrom_start != 0x00000000)
        {
            file_init(&texture_table[i].file);
        }
    }
}