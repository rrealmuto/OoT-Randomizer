CFG_DEADLY_BONKS:
	.word 0x00000000

APPLY_BONK_DAMAGE:
    addiu   sp, sp, -0x18
    sw      ra, 0x10($sp)

    ; displaced code
    or      a0, s0, $zero
    jal     0x80390B18  ; func_80838178
    nop

    ; One Bonk KO setting enabled
    lw      t0, CFG_DEADLY_BONKS
    beqz    t0, @@return_bonk
    nop

    ; Set player health to zero
    lui     t1, 0x8012              ; Save Context (upper half)
    addiu   t1, t1, 0xA5D0          ; Save Context (lower half)
    sh      $zero, 0x30(t1)

@@return_bonk:
    lw      ra, 0x10($sp)
    jr      ra
    addiu   sp, sp, 0x18