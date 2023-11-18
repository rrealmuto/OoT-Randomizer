; Hookshot hacks

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