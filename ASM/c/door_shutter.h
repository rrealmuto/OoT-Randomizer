#ifndef Z_DOOR_SHUTTER_H
#define Z_DOOR_SHUTTER_H

#include "z64.h"

/**
 * Actor Parameters
 *
 * |                  |         |
 * | Transition Index | Type    | Switch Flag
 * |------------------|---------|-------------
 * | 0 0 0 0 0 0      | 0 0 0 0 | 0 0 0 0 0 0
 * | 6                | 4       | 6
 * |
 *
 * Transition Index     1111110000000000    Set by the actor engine when the door is spawned
 * Type                 0000001111000000
 * Switch Flag          0000000000111111
 *
 */

#define DOORSHUTTER_GET_TYPE(actor)        PARAMS_GET_U((actor)->variable, 6, 4)
#define DOORSHUTTER_GET_SWITCH_FLAG(actor) PARAMS_GET_U((actor)->variable, 0, 6)

#define DOORSHUTTER_PARAMS(type, switchFlag) ((((type) & 0xF) << 6) | ((switchFlag) & 0x3F))

// DoorShutter and DoorGerudo share isActive
// Due to alignment, a substruct cannot be used in the structs of these actors.
#define SLIDING_DOOR_ACTOR_BASE      \
    /* 0x0000 */ DynaPolyActor dyna; \
    /* 0x0164 */ int16_t isActive // Set to true by player when using the door. Also a timer for niche cases in DoorShutter

typedef enum DoorShutterType {
    /* 0x00 */ SHUTTER,
    /* 0x01 */ SHUTTER_FRONT_CLEAR,
    /* 0x02 */ SHUTTER_FRONT_SWITCH,
    /* 0x03 */ SHUTTER_BACK_LOCKED,
    /* 0x04 */ SHUTTER_PG_BARS,
    /* 0x05 */ SHUTTER_BOSS,
    /* 0x06 */ SHUTTER_GOHMA_BLOCK,
    /* 0x07 */ SHUTTER_FRONT_SWITCH_BACK_CLEAR,
    /* 0x0B */ SHUTTER_KEY_LOCKED = 11
} DoorShutterType;

struct DoorShutter;

typedef void (*DoorShutterActionFunc)(struct DoorShutter*, z64_game_t*);

typedef struct DoorShutter {
    /* 0x0000 */ SLIDING_DOOR_ACTOR_BASE;
    /* 0x0166 */ int16_t jabuDoorClosedAmount; // Ranges from 0 (open) to 100 (closed)
    /* 0x0168 */ int16_t bossDoorTexIndex;
    /* 0x016A */ uint8_t doorType;
    /* 0x016B */ uint8_t styleType;
    /* 0x016C */ uint8_t gfxType;
    /* 0x016D */ int8_t requiredObjectSlot;
    /* 0x016E */ int8_t unlockTimer; // non-0 if the door is locked, ticks down while the door is unlocking
    /* 0x016F */ int8_t actionTimer;
    /* 0x0170 */ float barsClosedAmount; // Ranges from 0.0f (unbarred) to 1.0f (barred)
    /* 0x0174 */ DoorShutterActionFunc actionFunc;
} DoorShutter; // size = 0x0178

#endif
