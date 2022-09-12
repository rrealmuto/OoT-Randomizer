#include "pots.h"
#include "n64.h"
#include "gfx.h"
#include "textures.h"

#define DUNGEON_POT_SIDE_TEXTURE (uint8_t *)0x050108A0
#define DUNGEON_POT_DLIST (z64_gfx_t *)0x05017870

#define POT_SIDE_TEXTURE (uint8_t *)0x06000000
#define POT_DLIST (z64_gfx_t *)0x060017C0

extern uint8_t POTCRATE_TEXTURES_MATCH_CONTENTS;


override_t get_pot_override(z64_actor_t *actor, z64_game_t *game)
{
    // make sure that the pot is actually supposed to drop something
    // there are some pots w/ flags that don't drop anything
    uint8_t pot_item = (actor->variable & 0x3F);
    if (pot_item == 0x3F)
    {
        return (override_t){0};
    }

    // make a dummy EnItem00 with enough info to get the override
    EnItem00 dummy;
    dummy.collectibleFlag = (actor->variable & 0x7E00) >> 9;
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;
    dummy.actor.variable = pot_item;
    if (!should_override_collectible(&dummy))
    {
        return (override_t){0};
    }

    return lookup_override((z64_actor_t *)&dummy, game->scene_index, 0);
}

override_t get_flying_pot_override(z64_actor_t *actor, z64_game_t *game)
{
    EnItem00 dummy;
    dummy.collectibleFlag = (actor->variable & 0x3F);
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;
    dummy.actor.variable = 0;
    if (!should_override_collectible(&dummy))
    {
        return (override_t){0};
    }

    return lookup_override((z64_actor_t *)&dummy, game->scene_index, 0);
}

void draw_pot(z64_actor_t *actor, z64_game_t *game, override_t override)
{
    // get original dlist and texture
    z64_gfx_t *dlist = DUNGEON_POT_DLIST;
    uint8_t *side_texture = DUNGEON_POT_SIDE_TEXTURE;

    // overworld pot or hba pot
    if ((actor->actor_id == 0x111 && (actor->variable >> 8) & 1) || actor->actor_id == 0x117)
    {
        dlist = POT_DLIST;
        side_texture = POT_SIDE_TEXTURE;
    }

    // get override texture
    if(POTCRATE_TEXTURES_MATCH_CONTENTS == PTMC_UNCHECKED && override.key.all != 0)
    {
        side_texture = get_texture(TEXTURE_ID_POT_GOLD);
    }
    else if (POTCRATE_TEXTURES_MATCH_CONTENTS == PTMC_CONTENTS && override.key.all != 0)
    {
        uint16_t item_id = resolve_upgrades(override.value.item_id);
        item_row_t *row = get_item_row(override.value.looks_like_item_id);
        if (row == NULL) {
            row = get_item_row(override.value.item_id);
        }
        if (row->chest_type == GILDED_CHEST)
        {
            side_texture = get_texture(TEXTURE_ID_POT_GOLD);
        }
        else if (row->chest_type == SILVER_CHEST)
        {
            side_texture = get_texture(TEXTURE_ID_POT_KEY);
        }
        else if (row->chest_type == GOLD_CHEST)
        {
            side_texture = get_texture(TEXTURE_ID_POT_BOSSKEY);
        }
        else if (row->chest_type == SKULL_CHEST_SMALL || row->chest_type == SKULL_CHEST_BIG)
        {
            side_texture = get_texture(TEXTURE_ID_POT_SKULL);
        }
    }

    // push custom dlist (that sets the texture) to segment 09
    z64_gfx_t *gfx = game->common.gfx;
    gfx->poly_opa.d -= 2;
    gDPSetTextureImage(gfx->poly_opa.d, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, side_texture);
    gSPEndDisplayList(gfx->poly_opa.d + 1);
    gMoveWd(gfx->poly_opa.p++, G_MW_SEGMENT, 9 * sizeof(int), gfx->poly_opa.d);

    // draw the original dlist that has been hacked in ASM to jump to the custom dlist
    z64_Gfx_DrawDListOpa(game, dlist);
}

void draw_pot_hack(z64_actor_t *actor, z64_game_t *game)
{
    draw_pot(actor, game, get_pot_override(actor, game));
}

void draw_hba_pot_hack(z64_actor_t *actor, z64_game_t *game)
{
    EnGSwitch *switch_actor = (EnGSwitch *)actor;

    if (!switch_actor->broken)
    {
        draw_pot(actor, game, (override_t){0});
    }
}

void draw_flying_pot_hack(z64_actor_t *actor, z64_game_t *game)
{
    draw_pot(actor, game, get_flying_pot_override(actor, game));
}
