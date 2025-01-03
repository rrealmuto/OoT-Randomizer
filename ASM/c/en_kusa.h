#ifndef Z_EN_KUSA_H
#define Z_EN_KUSA_H

#include "z64.h"

struct EnKusa;

typedef void (*EnKusaActionFunc)(struct EnKusa*, z64_game_t*);

typedef enum {
    /* 0 */ ENKUSA_TYPE_0,
    /* 1 */ ENKUSA_TYPE_1,
    /* 2 */ ENKUSA_TYPE_2
} EnKusaType;

typedef struct EnKusa {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x013C */ EnKusaActionFunc actionFunc;
    /* 0x0140 */ ColliderCylinder collider;
    /* 0x018C */ int16_t timer;
    /* 0x018E */ int8_t objBankIndex;
    /* 0x0190 */ uint8_t chest_type;
} EnKusa; // size = 0x01A0 (we increased this by 0x10)

#endif
