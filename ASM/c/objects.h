#ifndef OBJECTS_H
#define OBJECTS_H

#include "z64.h"

#define OBJECT_INVALID 0
#define OBJECT_EXCHANGE_BANK_MAX 19
#define OBJECT_EXCHANGE_BANK_EXTENDED_COUNT 30
#define OBJECT_EXCHANGE_BANK_EXTENDED_MAX OBJECT_EXCHANGE_BANK_MAX + OBJECT_EXCHANGE_BANK_EXTENDED_COUNT

typedef struct extended_object {
    int16_t id;
    uint8_t is_active;
    void*   data;
} extended_object_t;

typedef struct extended_object_ctx {
    uint8_t* heap;
    uint8_t* free;
    uint32_t size;
    uint8_t num;
    int8_t holl_last_room; // Used to keep track of loading plane transition rooms to prevent unloading objects if they're still being used.
    uint8_t inhibit_clear_flag;
    extended_object_t slots[OBJECT_EXCHANGE_BANK_EXTENDED_MAX];
} extended_object_ctx_t;

void extended_objects_init();
void extended_objects_reset();
void enitem00_set_link_incoming_item_id(z64_actor_t *actor, z64_game_t *game, int32_t getItemId);
int32_t Object_GetIndex_Hook(z64_obj_ctxt_t *object_ctx, int16_t object_id);

#endif
