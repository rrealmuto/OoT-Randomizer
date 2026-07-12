.headersize(0x80b72bd0 - 0x00eee2f0)

; Hack Guays (en_crow) to not respawn in enemy drop shuffle
.org 0x80b73124     ; Beginning of EnCrow_SetupRespawn 0xEEE844
; replaces
;   or      a2, a0, r0
;   lw      t6, 0x0000(v0)
;   li      at, 10
;   li      t7, 1
    jal     en_crow_respawn_hack
    nop
    bnez    v1,0x80b731f8    ; if v1 = 1, guay was killed = jump to end of function, load ra
    nop
