#include "objects.h"

#include "z64.h"

#define OBJECT_EXCHANGE_BANK_MAX 19

int32_t object_index_or_spawn(z64_obj_ctxt_t *object_ctx, int16_t object_id)
{
    int32_t index = z64_ObjectIndex(object_ctx, object_id);
    if (index == -1 && object_ctx->n_objects < OBJECT_EXCHANGE_BANK_MAX)
    {
        // Track the number of spawned objects so the Object List
        // SceneCmd can clear out all subsequent objects and kill the
        // corresponding actors on room transition
        uint8_t n_spawned_objects = object_ctx->n_spawned_objects;
        index = z64_ObjectSpawn(object_ctx, object_id);
        object_ctx->n_spawned_objects = n_spawned_objects;
    }

    return index;
}
