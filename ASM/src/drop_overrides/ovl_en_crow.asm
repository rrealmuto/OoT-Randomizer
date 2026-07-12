; Hack Guays to not respawn when enemy drop shuffle is enabled
; A0 contains the actor
; actionFunc is at 0x01B0(A0) if we care.
en_crow_respawn_hack:
    addiu   sp,sp,-24
    sw      ra,16(sp)
    sw      a0,20(sp)    ; actor
    sw      a1,24(sp)    ; play
    sw      v0,28(sp)    ; loaded reloc address

; Check if the setting is enabled.
    lb      a1,CFG_PREVENT_GUAY_RESPAWNS
    beqzl   a1,@en_crow_return_to_func
    move    v1,zero     ; return 0 if should continue SetupRespawn/no kill
; Setting is enabled so kill the guay.
    jal     Actor_Kill
    nop
    li      v1,1        ; return 1 if Guay killed = return early
    
    lw      ra,16(sp)
    lw      a0,20(sp)
    lw      a1,24(sp)
    lw      v0,28(sp)
@en_crow_return_to_func:
    move    a2,a0       ; displaced
    lw      t6,(v0)     ; displaced
    li      at,10       ; displaced
    li      t7,1        ; displaced
    jr      ra
    addiu   sp,sp,24
