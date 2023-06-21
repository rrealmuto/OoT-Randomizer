; Skull Kid Enemy Drop Override
; Hack at call to Item_DropCollectible to set drop_collectible_override_flag
; Actor instance stored in s0
en_skj_drop_collectible_hack:
    addiu   sp, sp, -0x20
    sw      at, 0x10(sp)
    sw      v0, 0x14(sp)
    sw      ra, 0x18(sp)
    li      at, drop_collectible_override_flag
    lh      v0, 0x18(s0) ; Get flag from z rotation
    sh      v0, 0x00(at)
    jal     0x80013678 ; Call Item_DropCollectible
    nop
    lw      v0, 0x14(sp)
    lw      at, 0x10(sp)
    lw      ra, 0x18(sp)
    jr      ra
    addiu   sp, sp, 0x20