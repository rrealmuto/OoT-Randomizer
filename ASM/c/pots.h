#ifndef POTS_H
#define POTS_H

#include "item_table.h"
#include "get_items.h"
#include "z64.h"

override_t get_pot_override(z64_actor_t *actor, z64_game_t *game);
override_t get_flying_pot_override(z64_actor_t *actor, z64_game_t *game);

#endif
