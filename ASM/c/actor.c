#include "z64.h"

#define BG_HAKA_TUBO        0x00BB
#define BG_SPOT18_BASKET    0x015C
#define OBJ_COMB            0x19E
#define OBJ_MURE3           0x1AB
#define OBJ_TSUBO           0x0111
#define EN_ITEM00           0x0015
#define EN_G_SWITCH         0x0117 //Silver Rupee

extern uint8_t SHUFFLE_SILVER_RUPEES;

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

z64_actor_t* Actor_SpawnEntry_Hack(void* actorCtx, ActorEntry* actorEntry, z64_game_t* globalCtx){
    uint16_t newParams = actorEntry->params;
    uint16_t newID = actorEntry->id;

    switch(actorEntry->id)
    {
        case(EN_G_SWITCH):
        {
            if(SHUFFLE_SILVER_RUPEES)
            {
                uint8_t type = (actorEntry->params >> 0x0C) & 0xF;
                if(type == 1) // only override actual silver rupees, not the switches or pots.
                {
                    newParams = 0;
                    newID = EN_ITEM00;
                }
                break;
            }
        }
        default:
            break;
    }
    return z64_SpawnActor(actorCtx, globalCtx, newID, actorEntry->pos.x, actorEntry->pos.y, actorEntry->pos.z,
                       actorEntry->rot.x, actorEntry->rot.y, actorEntry->rot.z, newParams);
}