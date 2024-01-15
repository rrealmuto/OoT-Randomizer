Collider_SetCylinder:
    addiu   sp, sp, -0x18
    j       Collider_SetCylinder_AfterHook
    sw      ra, 0x14(sp)