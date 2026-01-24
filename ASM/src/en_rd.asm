; Check if gSaveContext.sunsSongState == SUNSSONG_SPECIAL
; We can use t9 because it gets set right after this
; Can also use t0
EnRd_SetSunsSongState:
    lh      t9, -0x460e(at)
    li      t0, 3
    beql    t9, t0, @@return
    sh      r0, -0x460e(at)
@@return:
    jr      ra
    nop

EnRd_UpdateCheckSunsSong:
    lh      t7, 0x1422(v0) ; t7 contains sunSongState
    jr      ra
    addiu   t8, r0, 3

EnRd_DestroyCheckSunsSong:
    or      a0, a3, r0 ; Replaced code
    jr      ra
    addiu   t7, r0, 3