#ifndef Z_OBJ_ROOMTIMER_H
#define Z_OBJ_ROOMTIMER_H

#include "z64.h"

struct ObjRoomtimer;

typedef void (*ObjRoomtimerActionFunc)(struct ObjRoomtimer*, z64_game_t*);

typedef struct ObjRoomtimer {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ ObjRoomtimerActionFunc actionFunc;
    /* 0x0150 */ uint32_t switchFlag;
} ObjRoomtimer; // size = 0x0154

#endif
