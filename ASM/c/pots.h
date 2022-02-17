#ifndef POTS_H
#define POTS_H

#include "item_table.h"
#include "get_items.h"
#include "z64.h"

extern uint32_t POT_TEXTURE_MATCH_CONTENTS;

override_t get_pot_override(z64_actor_t *actor, z64_game_t *game);

#endif
