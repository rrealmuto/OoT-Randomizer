#include "z64.h"
#include "actor.h"
#include "get_items.h"

// Hacked call at the beginning of enkusa_dropcollectible
// Use this for our grass shuffle drops
// Return 0 if we didn't override so it returns to the original EnKusa_DropCollectible function
// Return 1 if we overrode so it returns to the caller
int enkusa_dropcollectible_hack(z64_actor_t* actor, z64_game_t* game)
{
    // Get our flag from the actor
    ActorAdditionalData* extra = Actor_GetAdditionalData(actor);
    if(extra->flag.all && !Get_NewOverrideFlag(&(extra->flag))) {
        drop_collectible_override_flag = extra->flag;
        z64_Item_DropCollectible(game, &(actor->pos_world), 0);
        z64_bzero(&drop_collectible_override_flag, sizeof(drop_collectible_override_flag));
        return 1;
    }
    return 0;
}
