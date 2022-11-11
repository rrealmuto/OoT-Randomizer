#include "actor.h"
#include "get_items.h"
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

//Return 1 to not spawn the actor, 0 to spawn the actor
//If enemy drops setting is enabled, check if the flag for this actor hasn't been set and make sure to spawn it.
//Flag is the index of the actor in the actor spawn list, or -1 if this function is not being called at the room init.
//Parent will be set if called by Actor_SpawnAsChild
uint8_t Actor_Spawn_Clear_Check_Hack(z64_game_t* globalCtx, ActorInit* actorInit, int16_t flag, z64_actor_t* parent)
{
    //probably need to do something specific for anubis spawns because they use the spawner items. Maybe flare dancers too?
    if(actorInit->id == 0x00E0 && parent != NULL)
    {
        flag = parent->rot_init.z;
    }
    if((actorInit->category == ACTORCAT_ENEMY) && z64_Flags_GetClear(globalCtx, globalCtx->room_index))
    {
        //Check if this we're spawning an actor from the room's actor spawn list
        if(flag > 0)
        {
            flag |= globalCtx->room_index << 8;
            //Build a dummy override
            EnItem00 dummy;
            dummy.actor.actor_id = 0x15;
            dummy.actor.variable = 0;
            dummy.actor.rot_init.y = flag;
            //Check if this actor is in the override list
            if(lookup_override(&dummy, globalCtx->scene_index, 0).key.all != 0 && !(Get_CollectibleOverrideFlag(&dummy)>0))
            {
                return 0;
            }
            return 1;
        }
        
        return 1;
    }
    

    return 0;
}
