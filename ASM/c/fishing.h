#ifndef _FISHING_H_
#define _FISHING_H_

#include "z64.h"
#include "get_items.h"

typedef struct Fishing {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x013C */ uint8_t fish_unk_13C[0x4];
    /* 0x0140 */ uint8_t isLoach;
    /* 0x0141 */ uint8_t fish_unk_141[0x7];
    /* 0x0148 */ int16_t fishState;
    /* 0x014A */ int16_t fishStateNext;
    /* 0x014C */ int16_t stateAndTimer;
    /* 0x014E */ uint8_t fish_unk_14E[0x3F2];
    /* 0x0540 */ override_t override;
} Fishing; //size = 0x0570 (we added 0x30)

#endif
