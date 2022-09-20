#include "z64.h"

#define BG_HAKA_TUBO        0x00BB
#define BG_SPOT18_BASKET    0x015C
#define OBJ_COMB            0x19E
#define OBJ_MURE3           0x1AB

void Actor_SetWorldToHome_End(z64_actor_t *actor) {
    // Reset z rotation to 0 for any actors that we use it as flag space
    // bg_haka_tubo          0xBB
    // bg_spot18_basket      0x15C
    // obj_comb              0x19E
    // obj_mure3             0x1AB
    if (actor->actor_id == BG_HAKA_TUBO ||
        actor->actor_id == BG_SPOT18_BASKET ||
        actor->actor_id == OBJ_COMB ||
        actor->actor_id == OBJ_MURE3)
    {
        actor->rot_world.z = 0;
    }
}
