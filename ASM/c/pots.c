#include "pots.h"
#include "n64.h"
#include "gfx.h"

#define DUNGEON_POT_SIDE_TEXTURE 0x050108A0
#define DUNGEON_POT_DLIST 0x05017870

#define POT_SIDE_TEXTURE 0x06000000
#define POT_DLIST 0x060017C0


extern bool POTCRATE_TEXTURES_MATCH_CONTENTS;

extern void *GILDED_POT_SIDE_TEXTURE;
extern void *KEY_POT_SIDE_TEXTURE;
extern void *GOLD_POT_SIDE_TEXTURE;
extern void *SKULL_POT_SIDE_TEXTURE;

extern void func_8007E298(z64_gfx_t* gfx);
asm(".equ func_8007E298, 0x8007E298");
extern Mtx_t *write_matrix_stack_top(z64_gfx_t *gfx);
asm(".equ write_matrix_stack_top, 0x800AB900");

override_t get_pot_override(z64_actor_t *actor, z64_game_t *game) {
    
    uint8_t pot_item = (actor->variable & 0x3F); //make sure that the pot is actually supposed to drop something. There are some pots w/ flags that don't drop anything
    if(pot_item == 0x3F)
    {
        return (override_t){0};
    }
    
    //make a dummy EnItem00 with enough info to get the override
    EnItem00 dummy;
    dummy.collectibleFlag = (actor->variable & 0x7E00) >> 9;
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;

    

    if (!should_override_collectible(&dummy)) {
        return (override_t){0};
    }

    return lookup_override(&dummy, game->scene_index, 0);
}

override_t get_flying_pot_override(z64_actor_t *actor, z64_game_t* game)
{
    EnItem00 dummy;
    dummy.collectibleFlag = (actor->variable & 0x3F);
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;
    if (!should_override_collectible(&dummy)) {
        return (override_t){0};
    }

    return lookup_override(&dummy, game->scene_index, 0);
}

void draw_flying_pot_hack(z64_actor_t *actor, z64_game_t *game)
{
    draw_pot(actor, game, get_flying_pot_override(actor, game));
}

void draw_pot_hack(z64_actor_t *actor, z64_game_t *game)
{
    draw_pot(actor, game, get_pot_override(actor, game));
}

void draw_pot(z64_actor_t *actor, z64_game_t *game, override_t potOverride) {
    z64_gfx_t *gfx = game->common.gfx;

    func_8007E298(gfx);
    //write matrix
    Mtx_t *mtx = write_matrix_stack_top(gfx);
    gSPMatrix(gfx->poly_opa.p++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD);


    //get dlist and texture
    int dlist = DUNGEON_POT_DLIST;
    void* texture = DUNGEON_POT_SIDE_TEXTURE;

    if((actor->variable >> 8) & 1)
    {
        dlist = POT_DLIST;
        texture = POT_SIDE_TEXTURE;
        
    }
    
    if(POTCRATE_TEXTURES_MATCH_CONTENTS && potOverride.key.all != 0)
    {
        uint16_t itemID = resolve_upgrades(potOverride.value.item_id);
        item_row_t* row = get_item_row(itemID);
        if(row->chest_type == GILDED_CHEST)
        {
            texture = &GILDED_POT_SIDE_TEXTURE;
        }
        if(row->chest_type == SILVER_CHEST)
        {
            texture = &KEY_POT_SIDE_TEXTURE;
        }
        if(row->chest_type == GOLD_CHEST)
        {
            texture = &GOLD_POT_SIDE_TEXTURE;
        }
        if(row->chest_type == SKULL_CHEST_SMALL || row->chest_type == SKULL_CHEST_BIG)
        {
            texture = &SKULL_POT_SIDE_TEXTURE;
        }
    }

    gfx->poly_opa.d -= 2;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, texture);
    gSPEndDisplayList(gfx->poly_opa.d + 1);
    gMoveWd(gfx->poly_opa.p++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);

    gSPDisplayList(gfx->poly_opa.p++, dlist);
}
