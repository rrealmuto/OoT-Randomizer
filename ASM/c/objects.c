#include "objects.h"
#include "z64.h"
#include "get_items.h"
#include "item_table.h"
#include "models.h"
#include "util.h"

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

int8_t gPrevRoom;

void extended_objects_init() {
    //extended_object_ctx.heap = heap_alloc(0x20000);
    //extended_object_ctx.free = extended_object_ctx.heap;
    //extended_object_ctx.num = 0;
    //extended_object_ctx.size = 0x20000;
    //extended_object_ctx.holl_last_room = -1;
    //extended_object_ctx.inhibit_clear_flag = 0;
    for(int i = 0; i < OBJECT_EXCHANGE_BANK_EXTENDED_MAX; i++) {
        extended_object_ctx.slots[i].id = 0;
        extended_object_ctx.slots[i].is_active = 0;
        extended_object_ctx.slots[i].room = -1;
        extended_object_ctx.slots[i].numDependencies = 0;
        extended_object_ctx.slots[i].data = NULL;
    }
    gPrevRoom = -1;
}

void extended_objects_reset(z64_game_t* play) {
    //extended_object_ctx.free = extended_object_ctx.heap;
    //extended_object_ctx.num = 0;
    //extended_object_ctx.holl_last_room = -1;
    //extended_object_ctx.inhibit_clear_flag = 0;
    for(int i = 3; i < OBJECT_EXCHANGE_BANK_EXTENDED_MAX; i++) {
        extended_object_ctx.slots[i].id = 0;
        if(extended_object_ctx.slots[i].data) {
            Object_Free(&play->objectCtx, extended_object_ctx.slots[i].data);
        }
        extended_object_ctx.slots[i].is_active = 0;
        extended_object_ctx.slots[i].room = -1;
        extended_object_ctx.slots[i].numDependencies = 0;
        extended_object_ctx.slots[i].data = NULL;
    }
    gPrevRoom = -1;
}

void reset_object_reference_count() {
    for(int i = 0; i < OBJECT_EXCHANGE_BANK_EXTENDED_MAX; i++) {
        extended_object_ctx.slots[i].numDependencies = 0;
    }
}

typedef struct SCmdObjectList {
    /* 0x00 */ uint8_t  code;
    /* 0x01 */ uint8_t  length;
    /* 0x04 */ int16_t* data;
} SCmdObjectList;

void* ExtendedObject_HeapAlloc(extended_object_ctx_t* objectCtx, int16_t objectId) {
    ObjectTableEntry* objectFile = get_object_entry(objectId);
    uint32_t size;
    void* nextPtr;
    
    size = objectFile->vrom_end - objectFile->vrom_start;
    // Try to allocate on the object heap
    void* addr = ObjectArena_Malloc(size);
    if (addr == NULL) {
        // Not enough space in the main object space so allocate on zeldaarena
        addr = ZeldaArena_Malloc(size);
    }
    return addr;
}

void Object_HeapAllocNew(z64_obj_ctxt_t* objectCtx, int32_t slot, int16_t objectId, bool deferLoad) {
    z64_mem_obj_t* entry = &objectCtx->slots[slot];
    ObjectTableEntry* objectFile = get_object_entry(objectId);
    uint32_t size;
    void* nextPtr;

    if(deferLoad)
        entry->id = -objectId;
    else
        entry->id = objectId;
    entry->dmaRequest.vromAddr = 0;

    size = objectFile->vrom_end - objectFile->vrom_start;
    // Try to allocate on the object heap
    entry->data = ObjectArena_Malloc(size);
    if (entry->data == NULL) {
        // Not enough space in the main object space so allocate on zeldaarena
        entry->data = ZeldaArena_Malloc(size);
    }
}

void Object_Free(z64_obj_ctxt_t* objectCtx, void* addr) {
    if((addr >= objectCtx->obj_space_start && addr < objectCtx->obj_space_end)) {
        ObjectArena_Free(addr);
    }
    else {
        ZeldaArena_Free(addr);
    }
}

int32_t Object_SpawnPersistent_New(z64_obj_ctxt_t* objectCtx, int16_t objectId) {
    objectCtx->slots[objectCtx->numEntries].id = objectId;
    ObjectTableEntry* entry = get_object_entry(objectId);
    uint32_t size = entry->vrom_end - entry->vrom_start;

    Object_HeapAllocNew(objectCtx, objectCtx->numEntries, objectId, false);

    DmaMgr_RequestSync(objectCtx->slots[objectCtx->numEntries].data, entry->vrom_start, size);

    objectCtx->numEntries++;
    objectCtx->numPersistentEntries = objectCtx->numEntries;

    return objectCtx->numEntries - 1;
}

void Scene_CommandObjectList_New(z64_game_t* play, SCmdObjectList* cmd) {
    int32_t i;
    int32_t j;
    int32_t k;
    z64_mem_obj_t* entry;
    z64_mem_obj_t* invalidatedEntry;
    z64_mem_obj_t* entries;
    int16_t* objectListEntry = SEGMENTED_TO_VIRTUAL(cmd->data);
    void* nextPtr;

    k = 0;
    i = play->objectCtx.numPersistentEntries;
    entries = play->objectCtx.slots;
    entry = &play->objectCtx.slots[i];

    while (i < play->objectCtx.numEntries) {
        if (entry->id != *objectListEntry) {

            invalidatedEntry = &play->objectCtx.slots[i];
            for (j = i; j < play->objectCtx.numEntries; j++) {
                // Check if this slot has any active actor references. If so don't delete it.
                if(extended_object_ctx.slots[j].numDependencies == 0) {
                    invalidatedEntry->id = OBJECT_INVALID;
                    // Check if this was spawned on the main ObjectArena
                    Object_Free(&play->objectCtx, invalidatedEntry->data);
                    extended_object_ctx.slots[j].id = OBJECT_INVALID;
                    extended_object_ctx.slots[j].data = 0;
                }
                invalidatedEntry++;
                
            }

            Actor_KillAllWithMissingObject(play, &play->actor_ctxt);
            break;
        }

        i++;
        k++;
        objectListEntry++;
        entry++;
    }

    while (k < cmd->length) {
        if(Object_GetIndex(&play->objectCtx, *objectListEntry) < 0)
        {
            if (extended_object_ctx.slots[i].id == 0) {
                Object_HeapAllocNew(&play->objectCtx, i, *objectListEntry, true);
                i++;
                k++;
                objectListEntry++;
            }
            else {
                i++;
            }
        }
        else {
            k++;
            objectListEntry++;
        }
    }

    play->objectCtx.numEntries = i;
}

void Scene_CommandObjectList_Hook(z64_game_t* globalCtx, void* scene_command) {
    
    Scene_CommandObjectList_New(globalCtx, scene_command);
    // Copy the original table into the extended one
    for(int i = 0; i < OBJECT_EXCHANGE_BANK_MAX; i++)
    {
        extended_object_ctx.slots[i].id = globalCtx->objectCtx.slots[i].id;
        extended_object_ctx.slots[i].data = globalCtx->objectCtx.slots[i].data;
    }
}

typedef struct EnHoll {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x013C */ int16_t planeAlpha;
    /* 0x013E */ uint8_t side;
    /* 0x013F */ uint8_t resetBgCoverAlpha;
    /* 0x0140 */ void* actionFunc;
} EnHoll; // size = 0x0144

/*void EnHoll_Room_Change_Hack(z64_game_t* globalCtx, RoomContext* roomCtx, EnHoll* holl) {
    if((holl->actor.room_index == extended_object_ctx.holl_last_room) || (holl->actor.room_index == roomCtx->curRoom.num)) {
        extended_object_ctx.inhibit_clear_flag = 1;
    }
    z64_UnloadRoom(globalCtx, roomCtx);
    extended_object_ctx.holl_last_room = holl->actor.room_index;
    extended_object_ctx.inhibit_clear_flag = 0;
}*/

void Room_Change_Hook(z64_game_t* globalCtx, RoomContext* roomCtx) {
    gPrevRoom = roomCtx->prevRoom.num;
    Room_Change(globalCtx, roomCtx);
    
    //if(extended_object_ctx.inhibit_clear_flag)
    //    return;
    
    //if(prevRoom >= 0){
        extended_object_t* slot = &extended_object_ctx.slots[OBJECT_EXCHANGE_BANK_MAX];
        for(int i = OBJECT_EXCHANGE_BANK_MAX; i < OBJECT_EXCHANGE_BANK_EXTENDED_MAX; i++) {
            if(slot->data && slot->is_active) {
                slot->is_active = 0;
            }
            else if(slot->room != roomCtx->curRoom.num) { // Don't unload the object if it is for the current room.
                // The slot is no longer active so free the slot and the data from the heap
                Object_Free(&globalCtx->objectCtx, slot->data);
                slot->id = 0;
                slot->room = -1;
                slot->data = 0;
            }
            slot++;
        }
    //}
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
            // Allocate space on our heap
            extended_object_ctx.slots[free_index].data = ExtendedObject_HeapAlloc(&extended_object_ctx, object_id);
            //extended_object_ctx.slots[OBJECT_EXCHANGE_BANK_MAX + i].data = extended_object_ctx.free;
            
            // Load the object
            load_object_file(object_id, extended_object_ctx.slots[free_index].data);
            extended_object_ctx.slots[free_index].id = object_id;

            extended_object_ctx.slots[free_index].is_active = 1;
            extended_object_ctx.slots[free_index].room = z64_game.room_ctx.curRoom.num;
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

    // Run z64_ActorSetLinkIncomingItemId regardless of CFG_OBJECT_SYSTEM
    if (!z64_ActorOfferGetItem(&this->actor, game, incoming_item_id, 50.0f, 10.0f) && CFG_OBJECT_SYSTEM) {
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
        gSPSegment(gfx->poly_opa.p++, 0x06, z64_game.objectCtx.slots[actor->obj_bank_index].data);
        gSPSegment(gfx->poly_xlu.p++, 0x06, z64_game.objectCtx.slots[actor->obj_bank_index].data);
    }
}

void* THA_AllocTailAlign16(TwoHeadArena* tha, size_t size);

#define OBJECT_LINK_BOY_SIZE    0x37800
#define OBJECT_LINK_CHILD_SIZE  0x2CF80

uint32_t LinkObjectVanillaSizes[] = { OBJECT_LINK_BOY_SIZE, OBJECT_LINK_CHILD_SIZE };
extern int16_t gLinkObjectIds[];

Arena ObjectArena;

// Actually allocate an arena/heap
void* Object_InitContext_AllocSpace(TwoHeadArena* tha, size_t size) {
    // Resize for adult/child object size
    size -= LinkObjectVanillaSizes[z64_file.link_age];
    int16_t linkObjectId = gLinkObjectIds[z64_file.link_age];
    size += gObjectTable[linkObjectId].vrom_end - gObjectTable[linkObjectId].vrom_start;
    void* spaceStart = THA_AllocTailAlign16(tha, size);
    // Initialize an actual arena to use for spawning objects
    ObjectArena_Init(&ObjectArena, spaceStart, size);
    return spaceStart;
}

void ObjectArena_Init(Arena* objectArena, void* spaceStart, size_t size) {
    __osMallocInit(objectArena, spaceStart, size);
}

void* ObjectArena_Malloc(size_t size) {
    return __osMalloc(&ObjectArena, size);
}

void ObjectArena_Free(void* ptr) {
    __osFree(&ObjectArena, ptr);
}

void check_and_free_extended_objects() {
    reset_object_reference_count();
    // Loop through every actor and update its object reference count
    int actor_list_index = 0;
    for (actor_list_index = 0; actor_list_index < 12; actor_list_index++) {
        // Get the actor list for this actor
        z64_actor_t* curr = z64_game.actor_list[actor_list_index].first;
        while (curr != NULL) {
            if (curr->obj_bank_index < OBJECT_EXCHANGE_BANK_EXTENDED_MAX)
            {
                extended_object_ctx.slots[curr->obj_bank_index].numDependencies++;
            }
            curr = curr->next;
        }
    }
    //uint8_t numPersistent = z64_game.objectCtx.numPersistentEntries;
    // Free extended object slots with no actor dependencies
    extended_object_t* slot = &extended_object_ctx.slots[OBJECT_EXCHANGE_BANK_MAX];
    for (int i = 0; i < OBJECT_EXCHANGE_BANK_EXTENDED_COUNT; i++) {
        if (slot->numDependencies == 0 && (slot->id > 0) && slot->data) { 
            // No dependencies
            Object_Free(&z64_game.objectCtx, slot->data);
            slot->data = NULL;
            slot->id = 0;
            slot->room = -1;
            slot->is_active = 0;
        }
        slot++;
    }
}