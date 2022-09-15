Actor_SetWorldToHome_Hook:
    addiu   sp, sp, -0x20
    sw      ra, 0x1C (sp)
    jal     Actor_SetWorldToHome_End
    nop
    lw      ra, 0x1C (sp)
    jr      ra
    addiu   sp, sp, 0x20
