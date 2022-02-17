CFG_DEADLY_BONKS:
	.word 0x00000000


BONK_LAST_FRAME:
    addiu   sp, sp, -0x18
    sw      ra, 0x10($sp)

    ; displaced code
    or      a0, s0, $zero
    jal     0x80390B18  ; func_80838178, static location as part of player overlay
    nop

    ; One Bonk KO setting enabled
    lw      t0, CFG_DEADLY_BONKS
    beqz    t0, @@return_bonk_frame
    nop
    ; Set player health to zero
    jal     APPLY_BONK_DAMAGE
    nop

@@return_bonk_frame:
    lw      ra, 0x10($sp)
    jr      ra
    addiu   sp, sp, 0x18


SET_BONK_FLAG:
    ; displaced code
    or      a0, s0, $zero
    addiu   a1, $zero, 0x00FF

    ; One Bonk KO setting enabled
    lw      t0, CFG_DEADLY_BONKS
    beqz    t0, @@return_bonk_flag
    nop

    ; set flag
    lbu     t0, 0x0682(s0)   ; Player state3 flag 4
    ori     t1, t0, 0x0010
    sb      t1, 0x0682(s0)

@@return_bonk_flag:
    jr      ra
    nop


CHECK_FOR_BONK_CANCEL:
    addiu   sp, sp, -0x18
    sw      ra, 0x10($sp)

    ; displaced code
    lbu     v0, 0x0A63(s0)
    or      a1, s0, $zero

    ; Check if bonk flag was set and
    ; bonk animation flag (??) was cleared
    lbu     t0, 0x0682(s0)   ; Player state3 flag 4
    andi    t1, t0, 0x0010
    beqz    t1, @@return_bonk_check
    nop
    lh      t1, 0x0840(s0)   ; this->unk_850
    bnez    t1, @@return_bonk_check
    nop
    jal     APPLY_BONK_DAMAGE
    nop

@@return_bonk_check:
    lw      ra, 0x10($sp)
    jr      ra
    addiu   sp, sp, 0x18


APPLY_BONK_DAMAGE:
    ; Unset bonk kill flag
    lbu     t0, 0x0682(s0)   ; Player state3 flag 4
    andi    t1, t0, 0xFFEF
    sb      t1, 0x0682(s0)

    ; Set player health to zero
    lui     t1, 0x8012       ; Save Context (upper half)
    addiu   t1, t1, 0xA5D0   ; Save Context (lower half)
    lh      t2, 0x13C8(t1)   ; Nayru's Love Timer, range 0 - 1200
    bnez    t2, @@return_bonk
    nop
    sh      $zero, 0x30(t1)  ; Player Health

@@return_bonk:
    jr      ra

KING_DODONGO_BONKS:
    ; displaced code
    lh      t2, 0x0032(s1)
    mtc1    $zero, $f16

    ; One Bonk KO setting enabled
    lw      t0, CFG_DEADLY_BONKS
    beqz    t0, @@return_bonk_kd
    nop

    ; Set King Dodongo health to zero
    lh      t1, 0x0198(s0)          ; this->numWallCollisions
    beqz    t1, @@return_bonk_kd
    nop
    sh      $zero, 0x0184(s0)       ; this->health

@@return_bonk_kd:
    jr      ra