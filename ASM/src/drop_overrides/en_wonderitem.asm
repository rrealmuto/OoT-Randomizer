; a0 contains the list of tags
; v1 contains the index of the tag
; a1 contains the size of an entry in the list (0x0C)
; The lower half of the multiplaction register contains the result of multu v1, a1
; after this hack, the multiplication register needs to contain the result of multu v1, a1
EnWonderItem_Multitag_DrawHook:
    addiu   sp, -0x60
    sw      a0, 0x10(sp)
    sw      a1, 0x14(sp)
    sw      a2, 0x18(sp)
    sw      a3, 0x1C(sp)
    sw      at, 0x20(sp)
    sw      v0, 0x24(sp)
    sw      v1, 0x28(sp)
    sw      t0, 0x2c(sp)
    sw      t1, 0x30(sp)
    sw      t2, 0x34(sp)
    sw      t3, 0x38(sp)
    sw      t6, 0x3C(sp)
    sw      t7, 0x40(sp)
    swc1    f16,0x44(sp)
    sw      ra, 0x48(sp)
    or      a2, s0, r0 ; Copy the actor instance into a2
    jal     EnWonderItem_Multitag_DrawHack
    or      a1, v1, r0
    lw      a0, 0x10(sp)
    lw      a1, 0x14(sp)
    lw      a2, 0x18(sp)
    lw      a3, 0x1C(sp)
    lw      at, 0x20(sp)
    lw      v0, 0x24(sp)
    lw      v1, 0x28(sp)
    lw      t0, 0x2c(sp)
    lw      t1, 0x30(sp)
    lw      t2, 0x34(sp)
    lw      t3, 0x38(sp)
    lw      t6, 0x3C(sp)
    lw      t7, 0x40(sp)
    lwc1    f16,0x44(sp)
    multu   v1, a1
    lwc1    f4, 0x0024(a2)
    lwc1    f8, 0x0028(a2)
    lw      ra, 0x48(sp)
    jr      ra
    addiu   sp, 0x60



; a1 contains the list of tags
; a2 contains the index of the tag
; a1 contains the size of an entry in the list (0x0C)
; The lower half of the multiplaction register contains the result of multu v1, a1
; after this hack, the multiplication register needs to contain the result of multu v1, a1
EnWonderItem_MultitagOrdered_DrawHook:
    addiu   sp, -0x60
    sw      a0, 0x10(sp)
    sw      a1, 0x14(sp)
    sw      a2, 0x18(sp)
    sw      a3, 0x1C(sp)
    sw      at, 0x20(sp)
    sw      v0, 0x24(sp)
    sw      v1, 0x28(sp)
    sw      t0, 0x2c(sp)
    sw      t1, 0x30(sp)
    sw      t2, 0x34(sp)
    sw      t3, 0x38(sp)
    sw      t6, 0x3C(sp)
    sw      t7, 0x40(sp)
    swc1    f16,0x44(sp)
    sw      ra, 0x48(sp)
    or      a0, a1, r0
    or      a1, a2, r0
    jal     EnWonderItem_Multitag_DrawHack
    or      a2, s0, r0 ; Copy the actor instance into a2
    lw      a0, 0x10(sp)
    lw      a1, 0x14(sp)
    lw      a2, 0x18(sp)
    lw      a3, 0x1C(sp)
    lw      at, 0x20(sp)
    lw      v0, 0x24(sp)
    lw      v1, 0x28(sp)
    lw      t0, 0x2c(sp)
    lw      t1, 0x30(sp)
    lw      t2, 0x34(sp)
    lw      t3, 0x38(sp)
    lw      t6, 0x3C(sp)
    lw      t7, 0x40(sp)
    lwc1    f16,0x44(sp)
    multu   a2, t1
    lwc1    f4, 0x0024(v1)
    lwc1    f8, 0x0028(v1)
    lw      ra, 0x48(sp)
    jr      ra
    addiu   sp, 0x60