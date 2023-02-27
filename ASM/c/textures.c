#include "textures.h"


typedef struct {
    TextureId textureID;
    file_t file;
} texture_t;

texture_t texture_table[TEXTURE_ID_MAX] = {
    [TEXTURE_ID_POT_GOLD] = { .textureID = TEXTURE_ID_POT_GOLD },
    [TEXTURE_ID_POT_KEY] = { .textureID = TEXTURE_ID_POT_KEY },
    [TEXTURE_ID_POT_BOSSKEY] = { .textureID = TEXTURE_ID_POT_BOSSKEY },
    [TEXTURE_ID_POT_SKULL] = { .textureID = TEXTURE_ID_POT_SKULL },
    [TEXTURE_ID_CRATE_DEFAULT] = { .textureID = TEXTURE_ID_CRATE_DEFAULT },
    [TEXTURE_ID_CRATE_GOLD] = { .textureID = TEXTURE_ID_CRATE_GOLD },
    [TEXTURE_ID_CRATE_KEY] = { .textureID = TEXTURE_ID_CRATE_KEY },
    [TEXTURE_ID_CRATE_BOSSKEY] = { .textureID = TEXTURE_ID_CRATE_BOSSKEY },
    [TEXTURE_ID_CRATE_SKULL] = { .textureID = TEXTURE_ID_CRATE_SKULL },
    [TEXTURE_ID_SMALLCRATE_GOLD] = { .textureID = TEXTURE_ID_SMALLCRATE_GOLD },
    [TEXTURE_ID_SMALLCRATE_KEY] = { .textureID = TEXTURE_ID_SMALLCRATE_KEY },
    [TEXTURE_ID_SMALLCRATE_SKULL] = { .textureID = TEXTURE_ID_SMALLCRATE_SKULL },
    [TEXTURE_ID_SMALLCRATE_BOSSKEY] = { .textureID = TEXTURE_ID_SMALLCRATE_BOSSKEY },

    [TEXTURE_ID_CHEST_FRONT_GILDED] = { .textureID = TEXTURE_ID_CHEST_FRONT_GILDED },
    [TEXTURE_ID_CHEST_BASE_GILDED] = { .textureID = TEXTURE_ID_CHEST_BASE_GILDED },
    [TEXTURE_ID_CHEST_FRONT_SILVER] = { .textureID = TEXTURE_ID_CHEST_FRONT_SILVER },
    [TEXTURE_ID_CHEST_BASE_SILVER] = { .textureID = TEXTURE_ID_CHEST_BASE_SILVER },
    [TEXTURE_ID_CHEST_FRONT_SKULL] = { .textureID = TEXTURE_ID_CHEST_FRONT_SKULL },
    [TEXTURE_ID_CHEST_BASE_SKULL] = { .textureID = TEXTURE_ID_CHEST_BASE_SKULL },
};

uint8_t *get_texture(TextureId textureID) {
    return texture_table[textureID].file.buf;
}

void init_textures() {
    for (int i = 0; i < TEXTURE_ID_MAX; i++) {
        if (texture_table[i].file.vrom_start != 0x00000000) {
            file_init(&texture_table[i].file);
        }
    }
}
