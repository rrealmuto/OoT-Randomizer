#include "ovl_arms_hook.h"
#include "save.h"

extern uint8_t CONTINUOUS_HOOKSHOTS;
extern void ArmsHook_Wait(ArmsHook* this, z64_game_t* globalCtx);

void ArmsHook_Wait_Hooked(ArmsHook* this, z64_game_t* globalCtx) {

    // Call initial function
    // Set up the stack
    ArmsHook_Wait(this, globalCtx);
    // Calculate the new hookshot timer based on the hookshot level instead of heldItemAction
    if(CONTINUOUS_HOOKSHOTS) {
        this->timer = extended_savectx.hookshot_level * 13;
    }
}