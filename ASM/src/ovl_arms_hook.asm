; Arms_HookWait replaced code:
ArmsHook_Wait:
    addiu   sp, sp, -0x20
    sw      ra, 0x14(sp)
    li      gp, Arms_HookWait_Continue
    addiu   t9, r0, 0x66 ; Arms_Hook overlay
    j       Overlay_Call_Addr
    nop
