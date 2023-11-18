#include "ovl_arms_hook.h"
#include "save.h"

extern uint8_t HOOKSHOT_EXTENSIONS;
extern void ArmsHook_Wait(ArmsHook* this, z64_game_t* globalCtx);
extern void Player_DrawHookshotReticle(z64_game_t* globalCtx, z64_link_t* this, float arg2);

void ArmsHook_Wait_Hooked(ArmsHook* this, z64_game_t* globalCtx) {

    // Call initial function
    // Set up the stack
    ArmsHook_Wait(this, globalCtx);
    // Calculate the new hookshot timer based on the hookshot level instead of heldItemAction
    if(HOOKSHOT_EXTENSIONS) {
        this->timer = extended_savectx.hookshot_level * 13;
    }
}

void Player_DrawHookshotReticle_Hook(z64_game_t* globalCtx, z64_link_t* this, float arg2) {
    if(HOOKSHOT_EXTENSIONS) {
        arg2 = extended_savectx.hookshot_level * 38800.0;
    }
    Player_DrawHookshotReticle(globalCtx, this, arg2);
}
