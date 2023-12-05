#ifndef _FISHING_H_
#define _FISHING_H_

#include "z64.h"
#include "get_items.h"

typedef struct Fishing {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x013C */ uint8_t fish_unk[0x404];
    /* 0x0540 */ uint8_t pad[0x10];
    /* 0x0550 */ override_t override;
} Fishing; //size = 0x0550 (we added 0x10)

#endif
