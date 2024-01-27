#include "z64.h"
#include "actor.h"
#include "z64collision_check.h"
#include "scene.h"

extern int8_t curr_scene_setup;

typedef struct collidercylinder_override {
    uint8_t scene;
    union {
        uint32_t all;
        struct {
            uint8_t pad;
            uint8_t setup : 2;
            uint8_t room : 6;
            uint8_t flag;
        };
        struct {
            uint32_t pad: 8;
            uint32_t grotto_id: 5;
            uint32_t room: 4;
            uint32_t flag: 7;
        } grotto;
    };
    int16_t radius_override;
} collidercylinder_override;

collidercylinder_override collidercylinder_override_table[] = {
    { .scene = 0x58, .pad = 0, .setup = 2, .room = 1, .flag = 16, .radius_override = 80 }, // ZD Shop red ice
    { .scene = 0x09, .pad = 0, .setup = 0, .room = 3, .flag = 18, .radius_override = 80 }, // Ice cavern red ice wall
    { .scene = 0x09, .pad = 0, .setup = 0, .room = 3, .flag = 19, .radius_override = 80 }, // Ice cavern red ice wall
    { .scene = 0x00, .all = 0, .radius_override = 0 }
};

extern uint16_t CURR_ACTOR_SPAWN_INDEX;

int32_t Collider_SetCylinder_Hook(z64_game_t* globalCtx, ColliderCylinder* collider, z64_actor_t* actor, ColliderCylinderInit* src) {
    // check if our actor is in the collider override table
    collidercylinder_override* curr = &collidercylinder_override_table[0];
    ColliderCylinderInit copy_collider_init = *src;
    collidercylinder_override flag = { 0 };
    flag.scene = globalCtx->scene_index;
    flag.room = actor->room_index;
    flag.setup = curr_scene_setup;
    flag.flag = CURR_ACTOR_SPAWN_INDEX;
    while(curr->all)
    {
        if(curr->scene == flag.scene && curr->all == flag.all)
        {
            // Get the different between the new radius and the original
            float scale = (float)curr->radius_override / (float)src->dim.radius;
            copy_collider_init.dim.radius = curr->radius_override;
            // scale the actor
            actor->scale.x *= scale;
            actor->scale.y *= scale;
            actor->scale.z *= scale;
            break;
        }
        curr++;
    }
    
    return Collider_SetCylinder(globalCtx, collider, actor, &copy_collider_init);
}