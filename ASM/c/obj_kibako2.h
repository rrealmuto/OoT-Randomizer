#ifndef Z_OBJ_KIBAKO2_H
#define Z_OBJ_KIBAKO2_H

#include "item_table.h"
#include "get_items.h"
#include "z64.h"
struct ObjKibako2;

typedef void (*ObjKibako2ActionFunc)(struct ObjKibako2*, z64_game_t*);

typedef struct ObjKibako2 {
    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ uint8_t collider[0x4c];
    /* 0x01B0 */ void* actionFunc;
    /* 0x01B4 */ int16_t collectibleFlag;
} ObjKibako2; // size = 0x01B8

#endif
