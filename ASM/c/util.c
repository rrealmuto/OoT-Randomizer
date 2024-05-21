#include "util.h"
#include "z64.h"
#include "actor.h"

Arena randoArena;

extern char C_HEAP[];
extern char PAYLOAD_START[];
void* heap_next = NULL;


void heap_init() {
    __osMallocInit(&randoArena, (void*)C_HEAP, PAYLOAD_START - C_HEAP);
    //heap_next = &C_HEAP[0];
}

void* heap_alloc(int bytes) {
    void* ret = __osMalloc(&randoArena, bytes);
    char error_msg[256];
    if(ret == NULL) {
                
        uint32_t randoarena_maxFree;
        uint32_t randoarena_free;
        uint32_t randoarena_alloc;
        ArenaImpl_GetSizes(&randoArena, &randoarena_maxFree, &randoarena_free, &randoarena_alloc);
        sprintf(error_msg, "maxFree = %x\nfree = %x\nalloc = %x", (int)randoarena_maxFree, (int)randoarena_free, (int)randoarena_alloc);
        Fault_AddHungupAndCrashImpl("Rando arena overflow!", error_msg);
    }
    
    return ret;
}

void heap_free(void* ptr) {
    __osFree(&randoArena, ptr);
}

/*void *heap_alloc(int bytes) {
    int rem = bytes % 16;
    if (rem) bytes += 16 - rem;

    void* result = heap_next;
    heap_next = (char*)heap_next + bytes;
    return result;
}*/

void file_init(file_t* file) {
    file->buf = heap_alloc(file->size);
    read_file(file->buf, file->vrom_start, file->size);
}

void* resolve_overlay_addr(void* addr, uint16_t overlay_id) {
    ActorOverlay overlay = gActorOverlayTable[overlay_id];
    if (overlay.loadedRamAddr) {
        return addr - overlay.vramStart + overlay.loadedRamAddr;
    }
    return NULL;
}
