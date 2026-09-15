#include "z64.h"
#include "z64collision_check.h"

int32_t Collider_SetJntSph_ToActorWorld(z64_game_t* play, ColliderJntSph* dest, struct z64_actor_t* actor, ColliderJntSphInit* src, ColliderJntSphElement* jntSphElements) {
    // Call the original function
    Collider_SetJntSph(play, dest, actor, src, jntSphElements);
    // Move all the collider world coordinations to the actor's world pos
    z64_actor_t* thisX = (z64_actor_t*)actor;
    for(int i = 0; i < dest->count; i++) {
        dest->elements[i].dim.worldSphere.center.x = thisX->pos_world.x;
        dest->elements[i].dim.worldSphere.center.y = thisX->pos_world.y;
        dest->elements[i].dim.worldSphere.center.z = thisX->pos_world.z;
    }
}