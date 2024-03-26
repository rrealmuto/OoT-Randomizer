#ifndef MODELS_H
#define MODELS_H

#include "z64.h"

typedef struct {
    uint16_t object_id;
    uint8_t graphic_id;
} model_t;

typedef struct {
    uint16_t object_id;
    uint8_t* buf;
} loaded_object_t;

extern loaded_object_t bunny_hood_obj;

void models_init();
void models_reset();
void draw_model(model_t model, z64_actor_t* actor, z64_game_t* game, float base_scale);
void scale_top_matrix(float scale_factor);
loaded_object_t* get_object(uint32_t object_id);

#endif
