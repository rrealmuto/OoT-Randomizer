; Hack Guays to not respawn when enemy drop shuffle is enabled
; T7 contains address of the EnCrow_Respawn actionFunc.
; A2 contains the guay actor
; actionFunc is at 0x01B0(A2)
en_crow_respawn_hack:
    lb      a0, CFG_PREVENT_GUAY_RESPAWNS
    beqz    a0, @en_crow_set_actionfunc
    nop
@en_crow_set_actionfunc_nothing:
    li  t7, en_crow_donothing
@en_crow_set_actionfunc:
    sw  t7, 0x01B0(a2)

en_crow_donothing:
    jr  ra
    nop