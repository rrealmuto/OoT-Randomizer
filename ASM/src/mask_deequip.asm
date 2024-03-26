; Check if equipped mask = item in child trade slot before de-equipping mask.
mask_check_trade_slot:
    lui     v1, 0x8012          ; Save Context pt. 1
    lbu     t7, 0x014F(t0)      ; Equipped Mask Param
    addiu   v1, v1, 0xA5D0      ; Save Context pt. 2
    addiu   t7, t7, 0x23        ; Add 0x23 to equipped mask param to get item number.
    lbu     t8, 0x8B(v1)        ; Item in Child Trade Slot
    nop
    beq     t7, t8, @@return    ; If they are equal, skip de-equipping the mask.
    nop
    ; Check if we own the mask in the extended save bits. unk flags in scene 0x60
    li      v1, 0x8011A5D0 + 0xB64 ; Scene 0x60 unk flags address
    lw      v1, 0x00(v1)
    lbu     t7, 0x014F(t0)      ; Equipped Mask Param
    addiu   t7, t7, 2           ; Add 2 to convert from mask to trade flag pos
    addiu   t8, r0, 0x01
    sllv    t8, t8, t7          ; Shift 1 << mask param
    and     v1, v1, t8          ; bitwise AND the trade flags with the mask
    bnez    v1, @@return
    nop

    jr      ra
    sb      zero, 0x014F(t0)    ; De-equip Mask
    @@return:
    jr      ra
    nop
