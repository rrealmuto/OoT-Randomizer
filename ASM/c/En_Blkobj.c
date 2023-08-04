#include <stdint.h>
#include "z64.h"
#include "enemy_spawn_shuffle.h"

// Hack dark link room when it checks if dark link is still alive
// The function is normally checking if the return value of this is NULL so just return 1 if the soul flag isn't set.
z64_actor_t* En_Blkobj_Actor_Find_Hook(void* actorCtx, int32_t actorId, int32_t actorCategory)
{
    // Check for enemy soul shuffle
    if(CFG_ENEMY_SPAWN_SHUFFLE)
    {
        if(!flags_getsoul(47))
        {
            return 1;
        }
    }
    return z64_ActorFind(actorCtx, actorId, actorCategory);
}