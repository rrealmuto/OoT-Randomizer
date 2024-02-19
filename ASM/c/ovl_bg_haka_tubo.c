#include "get_items.h"
#include "z64.h"
#include "actor.h"

z64_actor_t* BgHakaTubo_DropCollectible_Hack(z64_actor_t* this, z64_xyzf_t* pos, uint16_t params, uint32_t index) {
    xflag_t* flag = &(Actor_GetAdditionalData(this)->flag);
    flag->subflag = index + 1;
    if (get_newflag_override(flag).key.all) {
        drop_collectible_override_flag = *flag;
        z64_actor_t* spawned = (z64_actor_t*)z64_Item_DropCollectible(&z64_game, pos, 0);
        z64_bzero(&drop_collectible_override_flag, sizeof(drop_collectible_override_flag));
        return spawned;
    }
    return (z64_actor_t*)z64_Item_DropCollectible(&z64_game, pos, params);
}
