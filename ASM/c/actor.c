#include "z64.h"
#include "actor.h"
#include "get_items.h"

#define BG_HAKA_TUBO        0x00BB
#define BG_SPOT18_BASKET    0x015C
#define OBJ_COMB            0x19E
#define OBJ_MURE3           0x1AB
#define OBJ_TSUBO           0x0111
#define EN_ITEM00           0x0015
#define EN_G_SWITCH         0x0117 //Silver Rupee

extern uint8_t SHUFFLE_SILVER_RUPEES;
extern uint16_t CURR_ACTOR_SPAWN_INDEX;
extern int8_t curr_scene_setup;

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
    bool continue_spawn = true;
    switch(actorEntry->id)
    {
        case(EN_G_SWITCH):
        {
            continue_spawn = spawn_override_silver_rupee(actorEntry, globalCtx);
            break;
        }
        default:
            break;
    }
    z64_actor_t* spawned = NULL;
    if(continue_spawn)
    {
        spawned = z64_SpawnActor(actorCtx, globalCtx, actorEntry->id, actorEntry->pos.x, actorEntry->pos.y, actorEntry->pos.z,
            actorEntry->rot.x, actorEntry->rot.y, actorEntry->rot.z, actorEntry->params);
    }
    return spawned;   
    
}

bool spawn_override_silver_rupee(ActorEntry* actorEntry, z64_game_t* globalCtx){
    if(SHUFFLE_SILVER_RUPEES)
    {
        EnItem00 dummy;
        dummy.actor.actor_id = 0x15;
        dummy.actor.rot_init.y = (curr_scene_setup << 14) + (globalCtx->room_index << 8) + CURR_ACTOR_SPAWN_INDEX + 1;
        uint8_t type = (actorEntry->params >> 0x0C) & 0xF;
        if(type == 1 && should_override_collectible(&dummy)) // only override actual silver rupees, not the switches or pots.
        {
            actorEntry->params = 0;
            actorEntry->id = EN_ITEM00;
            return true;
        }
        return false;
    }
    return true;
}