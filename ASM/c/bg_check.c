#include "z64.h"
#include "bg_check.h"

float BgCheck_EntityRaycastDown4_Raised(CollisionContext* colCtx, CollisionPoly** outGroundPoly, int32_t* bgId, z64_actor_t* actor, z64_xyzf_t* pos) {
    z64_xyzf_t new_pos = *pos;
    new_pos.y += 2.0; // Raise the starting position of the ray cast slightly. If it is already intersecting the floor, it'll skip the floor and clip down into the floor
    return BgCheck_EntityRaycastDown4(colCtx, outGroundPoly, bgId, actor, &new_pos);
}