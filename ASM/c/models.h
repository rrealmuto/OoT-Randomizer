#ifndef MODELS_H
#define MODELS_H

#include "z64.h"
#include "override.h"
#include <stdint.h>

typedef struct model_t {
    uint16_t object_id;
    uint8_t graphic_id;
} model_t;

typedef struct {
    uint16_t object_id;
    uint8_t *buf;
} loaded_object_t;

extern loaded_object_t bunny_hood_obj;

void models_init();
void models_reset();
void draw_model(model_t model, z64_actor_t *actor, z64_game_t *game, float base_scale);
void lookup_model_by_override(model_t *model, override_t override);
void fishing_draw(z64_actor_t* actor, z64_game_t* game) ;
uint32_t load_object_file(uint32_t object_id, uint8_t *buf);
uint32_t get_object_size(uint32_t object_id);
z64_object_table_t* get_object_entry(uint32_t object_id);
void scale_top_matrix(float scale_factor);
loaded_object_t* get_object(uint32_t object_id);

#endif
