Actor_UpdateAll_ClearRoomEnemyInhibit:
    addiu   sp, sp, -0x30
    sw      ra, 0x10(sp)
    sw      a0, 0x14(sp)
    sw      a1, 0x18(sp)
    sw      t6, 0x1C(sp)
    
    lui     t6, hi(curr_room_enemies_inhibited)
    sb      r0, lo(curr_room_enemies_inhibited)(t6)
    jal     reset_souls_inhibited
    nop
    ; Replaced code
    lw      v0, 0x0004(a1); rest of replaced code
    lui     t0, 0x8012
    lui     at, 0x00FF
    sll     t7, v0, 4
    srl     t8, t7, 28
    
    lw      ra, 0x10(sp)
    lw      a0, 0x14(sp)
    lw      a1, 0x18(sp)
    lw      t6, 0x1C(sp)
    jr      ra
    addiu   sp, sp, 0x30
