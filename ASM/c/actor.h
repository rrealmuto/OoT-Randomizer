#ifndef ACTOR_H
#define ACTOR_H
#include <stdbool.h>

void Actor_SetWorldToHome_End(z64_actor_t *actor);
z64_actor_t* Actor_SpawnEntry_Hack(void* actorCtx, ActorEntry* actorEntry, z64_game_t* globalCtx);
bool spawn_override_silver_rupee(ActorEntry* actorEntry, z64_game_t* globalCtx);

#endif
