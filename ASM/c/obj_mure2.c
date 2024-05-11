#include "z64.h"
#include "actor.h"
#include "get_items.h"

extern xflag_t* spawn_actor_with_flag;

z64_actor_t* ObjMure2_Actor_Spawn_Hack(z64_actor_t *this, z64_game_t *game, int actor_id, float x, float y, float z, uint16_t rx, uint16_t loop_index, uint16_t rz, uint16_t variable) {
    xflag_t flag = { 0 };
    ActorAdditionalData* extras = Actor_GetAdditionalData(this);
    Actor_BuildFlag(this, &flag, extras->actor_id, loop_index + 1);
    flag = resolve_alternative_flag(&flag);
    spawn_actor_with_flag = &flag;
    z64_actor_t* spawned = z64_SpawnActor(&(game->actor_ctxt), game, actor_id, x,y,z,rx,0,rz,variable);
    spawn_actor_with_flag = NULL;
    /*if(spawned) {
        ActorAdditionalData* spawnedExtras = Actor_GetAdditionalData(spawned);
        ActorAdditionalData* parentExtras = Actor_GetAdditionalData(this);
        if(parentExtras->flag.all) {
            spawnedExtras->flag = parentExtras->flag;
            spawnedExtras->flag.subflag = loop_index;
        }
    }*/
    return spawned;
}
