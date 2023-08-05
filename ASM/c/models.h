#ifndef MODELS_H
#define MODELS_H

#include <stdint.h>

typedef struct model_t {
    uint16_t object_id;
    uint8_t graphic_id;
} model_t;

typedef struct {
    uint16_t object_id;
    uint8_t *buf;
} loaded_object_t;

void models_init();
void models_reset();

#endif
