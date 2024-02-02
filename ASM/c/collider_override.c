#include "z64.h"
#include "actor.h"
#include "z64collision_check.h"
#include "scene.h"

extern int8_t curr_scene_setup;

typedef struct collidercylinder_override {
    xflag_t xflag;
    int16_t radius_override;
} collidercylinder_override;

collidercylinder_override collidercylinder_override_table[10];

extern uint16_t CURR_ACTOR_SPAWN_INDEX;
extern xflag_t* spawn_actor_with_flag;

int32_t Collider_SetCylinder_Hook(z64_game_t* globalCtx, ColliderCylinder* collider, z64_actor_t* actor, ColliderCylinderInit* src) {
    // check if our actor is in the collider override table
    collidercylinder_override* curr = &collidercylinder_override_table[0];
    ColliderCylinderInit copy_collider_init = *src;
    xflag_t flag = { 0 };
    if(CURR_ACTOR_SPAWN_INDEX) {
        flag.scene = globalCtx->scene_index;
        flag.room = actor->room_index;
        flag.setup = curr_scene_setup;
        flag.flag = CURR_ACTOR_SPAWN_INDEX;
        flag.subflag = 0;
    }
    else if(spawn_actor_with_flag) {
        flag = *spawn_actor_with_flag;
    }
    while(curr->xflag.all)
    {
        if(curr->xflag.scene == flag.scene && curr->xflag.all == flag.all)
        {
            // Get the different between the new radius and the original
            copy_collider_init.dim.radius = curr->radius_override;
            // scale the actor
            if(src->dim.radius != 0) { // check for 0 as crude fix for red ice wall colliders
                float scale = (float)curr->radius_override / (float)src->dim.radius;
                actor->scale.x *= scale;
                actor->scale.y *= scale;
                actor->scale.z *= scale;
            }
            
            break;
        }
        curr++;
    }
    
    return Collider_SetCylinder(globalCtx, collider, actor, &copy_collider_init);
}

