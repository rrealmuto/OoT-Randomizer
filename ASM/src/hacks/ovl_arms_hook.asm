; Hookshot hacks

; Hacks in arms_hook overlay
.headersize(0x8090fb40 - 0xcad2c0)

; Custom hookshot length function
; Hook Arms_HookWait Function

.org 0x8090fc08
; Replaces:
;   addiu   sp, sp, -0x20
;   sw      ra, 0x14(sp)
    j       ArmsHook_Wait_Hooked
    nop
Arms_HookWait_Continue:

; Hacks in code file related to hookshot
.headersize (0x800110A0 - 0xA87000)

; Hook call to Player_DrawHookshotReticle to set the distance for the reticle check
; At call to Player_DrawHookshotReticle in Player_PostLimbDrawGameplay
.org 0x8007bec8
; Replaces
;   jal   0x8007b560 ; call Player_DrawHookshotReticle
    jal   Player_DrawHookshotReticle_Hook
