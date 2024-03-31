item_menu_prevent_empty_equip:
    ; t1 = item under cursor
    ; a3 = slot?
    ; v0 - set to the usability value, or 0xFF to prevent equip
    
    bne     t1, 0xFF, @@check_usability ; Branch if item under cursor is not empty
    lbu     v0, 0x00 (s1) ; Load from usability table

    li      v0, 0xFF ; Prevent equip
    jr      ra
    li      at, 9

@@check_usability:
    j   item_menu_check_age_requirements_hook
    or  v0, r0, a3

;==================================================================================================

equipment_menu_prevent_empty_equip:
    addiu   sp, sp, -0x18
    sw      v0, 0x10 (sp)
    sw      ra, 0x14 (sp)

    jal     equipment_menu_slot_filled
    nop

    bnez    v0, @@return
    lbu     v1, 0x00 (t4) ; Load from usability table

    li      v1, 0xFF ; Prevent equip

@@return:
    lw      v0, 0x10 (sp)
    lw      ra, 0x14 (sp)
    addiu   sp, sp, 0x18
    jr      ra
    li      at, 9 ; Restore value expected by caller

;==================================================================================================

menu_use_blank_description:
    addiu   sp, sp, -0x18
    sw      v0, 0x10 (sp)
    sw      ra, 0x14 (sp)

    lhu     v0, 0x1E8 (s0) ; v0 = menu screen
    bne     v0, 3, @@not_equip_menu
    nop
    ; Check whether the equipment under the cursor has been obtained
    jal     equipment_menu_slot_filled
    nop
    bnez    v0, @@return ; Use default behavior if the equipment is obtained
    nop
    b       @@return
    li      v1, 0x7A ; 0x7A = index of texture that we made blank
@@not_equip_menu:

    ; Item menu: check whether the slot under the cursor is empty
    ; 0x17A is an invalid texture index, used if item ID = 0xFF
    bne     v1, 0x17A, @@return
    nop
    li      v1, 0x7A ; 0x7A = index of texture that we made blank

@@return:
    ; Displaced code
    sll     t4, v1, 10
    addu    a1, t4, t5

    lw      v0, 0x10 (sp)
    lw      ra, 0x14 (sp)
    jr      ra
    addiu   sp, sp, 0x18

;==================================================================================================

equipment_menu_slot_filled:
    addiu   sp, sp, -0x10
    sw      ra, 0x00 (sp)
    sw      v1, 0x04 (sp)
    sw      a0, 0x08 (sp)

    jal c_equipment_menu_slot_filled
    nop

    lw      ra, 0x00 (sp)
    lw      v1, 0x04 (sp)
    lw      a0, 0x08 (sp)
    jr      ra
    addiu   sp, sp, 0x10

equipment_menu_fix:
    and     t6, v1, t5
    bnez    t6, @@return
    lbu     t4, 0x0000 (t7) ; displaced
    addiu   ra, ra, 0x003C

@@return:
    jr      ra
    nop

item_menu_check_age_requirements_hook:
    addiu   sp, sp, -0x80
    sw      ra, 0x10(sp)
    sw      s0, 0x14(sp)
    sw      s1, 0x18(sp)
    sw      s2, 0x1C(sp)
    sw      s3, 0x20(sp)
    sw      s4, 0x24(sp)
    sw      s5, 0x28(sp)
    sw      s6, 0x2C(sp)
    sw      s7, 0x30(sp)
    sw      a0, 0x34(sp)
    sw      a1, 0x38(sp)
    sw      a2, 0x3C(sp)
    sw      t0, 0x40(sp)
    sw      t1, 0x44(sp)
    sw      t2, 0x48(sp)
    sw      t3, 0x4C(sp)
    sw      t4, 0x50(sp)
    sw      t5, 0x54(sp)
    sw      t6, 0x58(sp)
    sw      t7, 0x5C(sp)
    sw      t8, 0x60(sp)
    sw      t9, 0x64(sp)
    sw      at, 0x68(sp)
    sw      v1, 0x6C(sp)
    or      a0, v0, r0 ; Copy current item slot into 
    jal     check_slot_age_requirements
    nop
    lw      s0, 0x14(sp)
    lw      s1, 0x18(sp)
    lw      s2, 0x1C(sp)
    lw      s3, 0x20(sp)
    lw      s4, 0x24(sp)
    lw      s5, 0x28(sp)
    lw      s6, 0x2C(sp)
    lw      s7, 0x30(sp)
    lw      a0, 0x34(sp)
    lw      a1, 0x38(sp)
    lw      a2, 0x3C(sp)
    lw      t0, 0x40(sp)
    lw      t1, 0x44(sp)
    lw      t2, 0x48(sp)
    lw      t3, 0x4C(sp)
    lw      t4, 0x50(sp)
    lw      t5, 0x54(sp)
    lw      t6, 0x58(sp)
    lw      t7, 0x5C(sp)
    lw      t8, 0x60(sp)
    lw      t9, 0x64(sp)
    lw      at, 0x68(sp)
    lw      v1, 0x6C(sp)
    lw      ra, 0x10(sp)
    jr      ra
    addiu   sp, sp, 0x80

item_menu_check_age_requirements_item_hook:
    addiu   sp, sp, -0x80
    sw      ra, 0x10(sp)
    sw      s0, 0x14(sp)
    sw      s1, 0x18(sp)
    sw      s2, 0x1C(sp)
    sw      s3, 0x20(sp)
    sw      s4, 0x24(sp)
    sw      s5, 0x28(sp)
    sw      s6, 0x2C(sp)
    sw      s7, 0x30(sp)
    sw      a0, 0x34(sp)
    sw      a1, 0x38(sp)
    sw      a2, 0x3C(sp)
    sw      t0, 0x40(sp)
    sw      t1, 0x44(sp)
    sw      t2, 0x48(sp)
    sw      t3, 0x4C(sp)
    sw      t4, 0x50(sp)
    sw      t5, 0x54(sp)
    sw      t6, 0x58(sp)
    sw      t7, 0x5C(sp)
    sw      t8, 0x60(sp)
    sw      t9, 0x64(sp)
    sw      at, 0x68(sp)
    sw      v1, 0x6C(sp)
    or      a0, v0, r0 ; Copy current item slot into
    jal     check_item_age_requirements
    nop
    lw      s0, 0x14(sp)
    lw      s1, 0x18(sp)
    lw      s2, 0x1C(sp)
    lw      s3, 0x20(sp)
    lw      s4, 0x24(sp)
    lw      s5, 0x28(sp)
    lw      s6, 0x2C(sp)
    lw      s7, 0x30(sp)
    lw      a0, 0x34(sp)
    lw      a1, 0x38(sp)
    lw      a2, 0x3C(sp)
    lw      t0, 0x40(sp)
    lw      t1, 0x44(sp)
    lw      t2, 0x48(sp)
    lw      t3, 0x4C(sp)
    lw      t4, 0x50(sp)
    lw      t5, 0x54(sp)
    lw      t6, 0x58(sp)
    lw      t7, 0x5C(sp)
    lw      t8, 0x60(sp)
    lw      t9, 0x64(sp)
    lw      at, 0x68(sp)
    lw      v1, 0x6C(sp)
    lw      ra, 0x10(sp)
    jr      ra
    addiu   sp, sp, 0x80
