#include "ovl_arms_hook.h"

extern void ArmsHook_Wait(ArmsHook* this, z64_game_t* globalCtx);

void ArmsHook_Wait_Hooked(ArmsHook* this, z64_game_t* globalCtx) {

    // Call initial function
    // Set up the stack
    ArmsHook_Wait(this, globalCtx);
    this->timer = 100;
}