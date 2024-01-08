#include "util.h"
#include "z64.h"

Arena randoArena;

extern char C_HEAP[];
void* heap_next = NULL;

void heap_init() {
    __osMallocInit(&randoArena, (void*)C_HEAP, 0x1FF000);
    //heap_next = &C_HEAP[0];
}

void* heap_alloc(int bytes) {
    return __osMalloc(&randoArena, bytes);
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
