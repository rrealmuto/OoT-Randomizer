#include "z64.h"
#include "actor.h"

extern xflag_t* spawn_actor_with_flag;

z64_actor_t* EnShopnuts_ActorSpawnHack(z64_actor_t* this, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int16_t rotX, int16_t rotY, int16_t rotZ, int16_t params) {
    ActorAdditionalData* extras = Actor_GetAdditionalData(this);
    xflag_t flag = { 0 };
    Actor_BuildFlag(this, &flag, extras->actor_id, 0);
    spawn_actor_with_flag = &flag;
    z64_actor_t* spawned = z64_SpawnActor(&globalCtx->actor_ctxt, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
    spawn_actor_with_flag = NULL;
    return spawned;
}