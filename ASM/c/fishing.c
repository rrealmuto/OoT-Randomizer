#include "z64.h"
#include "get_items.h"
#include "scene.h"
#include "actor.h"
#include "models.h"
#include "fishing.h"


extern xflag_t* spawn_actor_with_flag;

z64_actor_t* Fishing_Actor_Spawn_Hook(void* actorCtx, z64_game_t* globalCtx, int16_t actorId, float posX, float posY, float posZ, int16_t rotX, int16_t rotY, int16_t rotZ, int16_t params) {
    xflag_t fish_flag = { 0 };
    fish_flag.room = globalCtx->room_index;
    fish_flag.scene = globalCtx->scene_index;
    fish_flag.setup = curr_scene_setup;
    fish_flag.flag = globalCtx->link_age; // Use the current age as the flag to distinguish child/adult fish
    fish_flag.subflag = params - 100; // Params contains the value 100 + i from the spawn loop. Add 1
    spawn_actor_with_flag = &fish_flag;
    Fishing* fish = z64_SpawnActor(actorCtx, globalCtx, actorId, posX, posY, posZ, rotX, rotY, rotZ, params);
    fish->override = get_newflag_override(&fish->actor, globalCtx);
    spawn_actor_with_flag = NULL;
    return fish;
}

extern void Fishing_DrawFish(z64_actor_t* this, z64_game_t* globalCtx);
void Fishing_DrawFish_Hook(z64_actor_t* this, z64_game_t* globalCtx) {
    Fishing* fish = (Fishing*)this;
    if(fish->override.key.all) {
        model_t model;
        lookup_model_by_override(&model, ((Fishing*)this)->override);
        draw_model(model, this, globalCtx, 1);
        return;
    }
    

    Fishing_DrawFish(this, globalCtx);
}