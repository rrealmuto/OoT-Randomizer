#include "blue_warp.h"
#include "z64.h"

#define TEXT_STATE_CLOSING 2

// TODO: Make rando flag to check to see if dungeon reward has been collected yet
#define HAS_COLLECTED_DUNGEON_REWARD 0

// Original function copied over
int32_t DoorWarp1_PlayerInRange(z64_actor_t *actor, z64_game_t *game) {
    if (actor->xzdist_from_link < 60.0f) {
        if ((z64_link.common.pos_world.y - 20.0f) < actor->pos_world.y) {
            if (actor->pos_world.y < (z64_link.common.pos_world.y + 20.0f)) {
                return 1;
            }
        }
    }
    return 0;
};

// Routine taken from Majora's Mask blue warps
int32_t DoorWarp1_PlayerInRange_Overwrite(z64_actor_t *actor, z64_game_t *game) {
    int32_t ret = 0;

    // Check vanilla range
    if (DoorWarp1_PlayerInRange(actor, game)) {

        if (HAS_COLLECTED_DUNGEON_REWARD) {
            return 1;
        }

        // Null out blue warp parent if it is still the dungeon boss
        if (z64_ActorHasParent(actor, game) && (actor->parent != &z64_link.common)) {
            actor->parent = NULL;
        }

        // Link will attach as the parent actor once the GetItem is accepted. Until then, offer the dungeon reward for Link.
        if (!z64_ActorHasParent(actor, game)) {
            // Put a dummy blue rupee on the blue warp, which will be overwritten by the medallions* (TODO)
            z64_ActorOfferGetItem(actor, game, 0x4D, 60.0f, 20.0f);
            return 0;
        } 

        // Wait until Link closes the textbox displaying the getItem reward
        if (z64_MessageGetState(((uint8_t *)(&z64_game)) + 0x20D8) == TEXT_STATE_CLOSING) {
            return 1;
        }
    }


    return ret;
}