#include "z64.h"

#define BG_HAKA_TUBO        0x00BB
#define BG_SPOT18_BASKET    0x015C
#define OBJ_COMB            0x19E
#define OBJ_MURE3           0x1AB
#define OBJ_TSUBO           0x0111
#define EN_ITEM00           0x0015

void Actor_SetWorldToHome_End(z64_actor_t *actor) {
    // Reset rotations to 0 for any actors that it gets passed into the spawn params
    // bg_haka_tubo          0xBB
    // bg_spot18_basket      0x15C
    // obj_mure3             0x1AB
    switch(actor->actor_id){
        case BG_HAKA_TUBO:
        case BG_SPOT18_BASKET:
        case OBJ_MURE3:
        case OBJ_COMB:
            actor->rot_world.z = 0;
            break;
        case EN_ITEM00:
            actor->rot_world.y = 0;
        default:
            break;
    } 
}
