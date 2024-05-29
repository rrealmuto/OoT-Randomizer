#include "objects.h"
#include "z64.h"
#include "get_items.h"
#include "models.h"

/*
 Bit-mapped configuration for new object system.
| 7       | 6            2 |             1 |                 0 |
| ENABLE  |     pad        | DOGS_ANYWHERE | FIX_BROKEN_ACTORS |
*/

#define ENABLE_OBJECT_SYSTEM_FLAG 0x80
#define DOGS_ANYWHERE             0x02
#define FIX_BROKEN_ACTORS         0x01

extern uint8_t CFG_OBJECT_SYSTEM;

extended_object_ctx_t extended_object_ctx;

void extended_objects_init() {
    //extended_object_ctx.heap = heap_alloc(0x20000);
    //extended_object_ctx.free = extended_object_ctx.heap;
    //extended_object_ctx.num = 0;
    //extended_object_ctx.size = 0x20000;
    extended_object_ctx.holl_last_room = -1;
    extended_object_ctx.inhibit_clear_flag = 0;
    for(int i = 0; i < OBJECT_EXCHANGE_BANK_EXTENDED_MAX; i++) {
        extended_object_ctx.slots[i].id = 0;
        extended_object_ctx.slots[i].is_active = 0;
        extended_object_ctx.slots[i].data = NULL;
    }
}

void extended_objects_reset() {
    //extended_object_ctx.free = extended_object_ctx.heap;
    //extended_object_ctx.num = 0;
    extended_object_ctx.holl_last_room = -1;
    extended_object_ctx.inhibit_clear_flag = 0;
    for(int i = OBJECT_EXCHANGE_BANK_MAX; i < OBJECT_EXCHANGE_BANK_EXTENDED_MAX; i++) {
        extended_object_ctx.slots[i].id = 0;
        if(extended_object_ctx.slots[i].data) {
            heap_free(extended_object_ctx.slots[i].data);
        }
        extended_object_ctx.slots[i].is_active = 0;
        extended_object_ctx.slots[i].data = NULL;
    }
}

void Scene_CommandObjectList_Hook(z64_game_t* globalCtx, void* scene_command) {
    Scene_CommandObjectList(globalCtx, scene_command);
    // Copy the original table into the extended one
    for(int i = 0; i < OBJECT_EXCHANGE_BANK_MAX; i++)
    {
        extended_object_ctx.slots[i].id = globalCtx->obj_ctxt.objects[i].id;
        extended_object_ctx.slots[i].data = globalCtx->obj_ctxt.objects[i].data;
    }
}

typedef struct EnHoll {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x013C */ int16_t planeAlpha;
    /* 0x013E */ uint8_t side;
    /* 0x013F */ uint8_t resetBgCoverAlpha;
    /* 0x0140 */ void* actionFunc;
} EnHoll; // size = 0x0144

void EnHoll_Room_Change_Hack(z64_game_t* globalCtx, RoomContext* roomCtx, EnHoll* holl) {
    if((holl->actor.room_index == extended_object_ctx.holl_last_room) || (holl->actor.room_index == roomCtx->curRoom.num)) {
        extended_object_ctx.inhibit_clear_flag = 1;
    }
    z64_UnloadRoom(globalCtx, roomCtx);
    extended_object_ctx.holl_last_room = holl->actor.room_index;
    extended_object_ctx.inhibit_clear_flag = 0;
}

void Room_Change_Hook(z64_game_t* globalCtx, RoomContext* roomCtx) {
    int8_t prevRoom = roomCtx->prevRoom.num;
    Room_Change(globalCtx, roomCtx);
    
    if(extended_object_ctx.inhibit_clear_flag)
        return;
    
    if(prevRoom >= 0){
        extended_object_t* slot = &extended_object_ctx.slots[OBJECT_EXCHANGE_BANK_MAX];
        for(int i = OBJECT_EXCHANGE_BANK_MAX; i < OBJECT_EXCHANGE_BANK_EXTENDED_MAX; i++) {
            if(slot->data && slot->is_active) {
                slot->is_active = 0;
            }
            else {
                // The slot is no longer active so free the slot and the data from the heap
                heap_free(slot->data);
                slot->id = 0;
                slot->data = 0;
            }
            slot++;
        }
    }
    
}

int32_t Object_GetIndex_Hook(z64_obj_ctxt_t *object_ctx, int16_t object_id) {
    int32_t index = Object_GetIndex(object_ctx, object_id);
    int32_t free_index = -1;
    if (index == -1) {
        if(object_id == 1 || object_id == 2) {
            // Don't spawn gameplay_field/dungeon keep on our extended space
            return index;
        }
        // Check if the object is in our table already
        for(int i = OBJECT_EXCHANGE_BANK_MAX; i < OBJECT_EXCHANGE_BANK_EXTENDED_MAX; i++) {
            if(free_index < 0 && extended_object_ctx.slots[i].id == 0) {
                free_index = i;
            }
            if (extended_object_ctx.slots[i].id == object_id) {
                extended_object_ctx.slots[i].is_active = 1;
                return i;
            }
        }
        // We didn't find the object so spawn it in our extended space
        if (free_index >= 0) {
            // Spawn the object
            // Figure out how much space we need
            uint32_t size = get_object_size(object_id);
            // Allocate space on our heap
            extended_object_ctx.slots[free_index].data = heap_alloc(size);
            //extended_object_ctx.slots[OBJECT_EXCHANGE_BANK_MAX + i].data = extended_object_ctx.free;
            
            // Load the object
            size = load_object_file(object_id, extended_object_ctx.slots[free_index].data);
            extended_object_ctx.slots[free_index].id = object_id;
            extended_object_ctx.slots[free_index].is_active = 1;
            //extended_object_ctx.free += size;
            //extended_object_ctx.num++;
            return free_index;
        }
    }
    return index;
}

// Hacked call to Object_GetIndex when the Player actor tries to spawn a dog
// Check if the dogs anywhere setting is enabled, and if it is, call our hacked version of Object_GetIndex
// Otherwise just call the original
int32_t Object_GetIndex_EnDog(z64_obj_ctxt_t *object_ctx, int16_t object_id) {
    if(CFG_OBJECT_SYSTEM & DOGS_ANYWHERE) {
        return Object_GetIndex_Hook(object_ctx, object_id);
    }
    return Object_GetIndex(object_ctx, object_id);
}

// Fix autocollect magic jar wonder items
void enitem00_set_link_incoming_item_id(z64_actor_t* actor, z64_game_t* game, int32_t incoming_item_id) {
    EnItem00* this = (EnItem00*)actor;

    // Run z64_ActorOfferGetItem regardless of CFG_OBJECT_SYSTEM
    if (!z64_ActorOfferGetItem(&this->actor, game, incoming_item_id, 50.0f, 10.0f) && CFG_OBJECT_SYSTEM) {
        switch (incoming_item_id) {
            case 0x43: // GI_MAGIC_SMALL
                z64_GiveItem(game, Z64_ITEM_MAGIC_SMALL);
                break;
            case 0x44: // GI_MAGIC_LARGE
                z64_GiveItem(game, Z64_ITEM_MAGIC_LARGE);
                break;
        }
    }
}

int32_t Object_IsLoaded_Hook(z64_obj_ctxt_t* objectCtx, int32_t bankIndex) {
    if (bankIndex >= OBJECT_EXCHANGE_BANK_MAX)
    {
        return extended_object_ctx.slots[bankIndex].data != 0;
    }
    return Object_IsLoaded(objectCtx, bankIndex);
}

void Actor_SetObjectDependency_Hook(z64_game_t* globalCtx, z64_actor_t* actor) {
    if (actor->obj_bank_index >= OBJECT_EXCHANGE_BANK_MAX) {
        z64_segments[6] = (uintptr_t)((extended_object_ctx.slots[actor->obj_bank_index].data) - 0x80000000);
    }
    else {
        Actor_SetObjectDependency(globalCtx, actor);
    }
}

void Actor_Draw_gSPSegment_Hack(z64_actor_t* actor) {
    z64_gfx_t *gfx = z64_game.common.gfx;
    
    if (actor->obj_bank_index >= OBJECT_EXCHANGE_BANK_MAX) {
        gSPSegment(gfx->poly_opa.p++, 0x06, extended_object_ctx.slots[actor->obj_bank_index].data);
        gSPSegment(gfx->poly_xlu.p++, 0x06, extended_object_ctx.slots[actor->obj_bank_index].data);
    }
    else {
        gSPSegment(gfx->poly_opa.p++, 0x06, z64_game.obj_ctxt.objects[actor->obj_bank_index].data);
        gSPSegment(gfx->poly_xlu.p++, 0x06, z64_game.obj_ctxt.objects[actor->obj_bank_index].data);
    }
}