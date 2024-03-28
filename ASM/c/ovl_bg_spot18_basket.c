#include "get_items.h"
#include "z64.h"
#include "actor.h"

z64_actor_t* BgSpot18Basket_BombDropHook(z64_actor_t* this, z64_xyzf_t* pos, uint32_t index) {
    xflag_t flag = Actor_GetAdditionalData(this)->flag;
    if (flag.all) {
        flag.subflag = index;
        if (!Get_NewOverrideFlag(&flag)) {
            drop_collectible_override_flag = flag;
            z64_actor_t* spawned = (z64_actor_t*)z64_Item_DropCollectible(&z64_game, pos, 0);
            z64_bzero(&drop_collectible_override_flag, sizeof(drop_collectible_override_flag));
            return spawned;
        }
    }
    return (z64_actor_t*)z64_Item_DropCollectible(&z64_game, pos, 4);
}

z64_actor_t* BgSpot18Basket_RupeeDropHook(z64_actor_t* this, z64_xyzf_t* pos, uint32_t index) {
    xflag_t flag = Actor_GetAdditionalData(this)->flag;
    if (flag.all) {
        flag.subflag = index + 3;
        if (!Get_NewOverrideFlag(&flag)) {
            drop_collectible_override_flag = flag;
            z64_actor_t* spawned = (z64_actor_t*)z64_Item_DropCollectible(&z64_game, pos, 0);
            z64_bzero(&drop_collectible_override_flag, sizeof(drop_collectible_override_flag));
            return spawned;
        }
    }

    return (z64_actor_t*)z64_Item_DropCollectible(&z64_game, pos, 0);
}

z64_actor_t* BgSpot18Basket_Heartpiecerupee_DropHook(z64_actor_t* this, z64_xyzf_t* pos, uint16_t params) {
    xflag_t flag = Actor_GetAdditionalData(this)->flag;
    if (flag.all) {
        flag.subflag = params + 5;
        if (!Get_NewOverrideFlag(&flag)) {
            drop_collectible_override_flag = flag;
            z64_actor_t* spawned = (z64_actor_t*)z64_Item_DropCollectible(&z64_game, pos, 0);
            z64_bzero(&drop_collectible_override_flag, sizeof(drop_collectible_override_flag));
            return spawned;
        }
    }

    return (z64_actor_t*)z64_Item_DropCollectible(&z64_game, pos, params);
}
