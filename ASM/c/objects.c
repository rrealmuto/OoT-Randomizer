#include "objects.h"
#include "z64.h"
#include "get_items.h"
#include "item_table.h"

extern uint8_t FIX_BROKEN_DROPS;

#define OBJECT_EXCHANGE_BANK_MAX 19

int32_t object_index_or_spawn(z64_obj_ctxt_t* object_ctx, int16_t object_id) {
    int32_t index = z64_ObjectIndex(object_ctx, object_id);
    if (index == -1 && object_ctx->n_objects < OBJECT_EXCHANGE_BANK_MAX) {
        // Track the number of spawned objects so the Object List
        // SceneCmd can clear out all subsequent objects and kill the
        // corresponding actors on room transition
        uint8_t n_spawned_objects = object_ctx->n_spawned_objects;
        index = z64_ObjectSpawn(object_ctx, object_id);
        object_ctx->n_spawned_objects = n_spawned_objects;
    }

    return index;
}

// Fix autocollect magic jar wonder items
void enitem00_set_link_incoming_item_id(z64_actor_t* actor, z64_game_t* game, int32_t incoming_item_id) {
    EnItem00* this = (EnItem00*)actor;

    // Run z64_ActorOfferGetItem regardless of FIX_BROKEN_DROPS
    if (!z64_ActorOfferGetItem(&this->actor, game, incoming_item_id, 50.0f, 10.0f) && FIX_BROKEN_DROPS) {
        switch (incoming_item_id) {
            case GI_MAGIC_JAR_SMALL: // GI_MAGIC_SMALL
                z64_GiveItem(game, Z64_ITEM_MAGIC_SMALL);
                break;
            case GI_MAGIC_JAR_LARGE: // GI_MAGIC_LARGE
                z64_GiveItem(game, Z64_ITEM_MAGIC_LARGE);
                break;
        }
    }
}

void* THA_AllocTailAlign16(TwoHeadArena* tha, size_t size);

#define OBJECT_LINK_BOY_SIZE    0x37800
#define OBJECT_LINK_CHILD_SIZE  0x2CF80

uint32_t LinkObjectVanillaSizes[] = { OBJECT_LINK_BOY_SIZE, OBJECT_LINK_CHILD_SIZE };
extern int16_t gLinkObjectIds[];

void* Object_InitContext_AllocSpace(TwoHeadArena* tha, size_t size) {
    // Resize for adult/child object size
    size -= LinkObjectVanillaSizes[z64_file.link_age];
    int16_t linkObjectId = gLinkObjectIds[z64_file.link_age];
    size += z64_object_table[linkObjectId].vrom_end - z64_object_table[linkObjectId].vrom_start;
    size += 0x30000;
    return THA_AllocTailAlign16(tha, size);
}