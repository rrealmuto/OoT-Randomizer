#ifndef OVL_ARMS_HOOK_H
#define OVL_ARMS_HOOK_H

#include "z64.h"

struct ArmsHook;

typedef void (*ArmsHookActionFunc)(struct ArmsHook*, z64_game_t*);
typedef void(*ArmsHook_Wait_Continue_Func)(struct ArmsHook*, z64_game_t*);

typedef struct ArmsHook {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x013C */ ColliderQuad collider;
    /* 0x01BC */ WeaponInfo hookInfo;
    /* 0x01D8 */ z64_xyzf_t unk_1E8;
    /* 0x01E4 */ z64_xyzf_t unk_1F4;
    /* 0x0190 */ z64_actor_t* grabbed;
    /* 0x0194 */ z64_xyzf_t grabbedDistDiff;
    /* 0x0200 */ int16_t timer;
    /* 0x0204 */ ArmsHookActionFunc actionFunc;
} ArmsHook; // size = 0x0208

#endif
