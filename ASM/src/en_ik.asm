; Hack to store additional param data in en_ik
; params passed into actor are stored in t9
EnIk_Params_Hack:
    andi    t2, t9, 0x80 ; Get the additional bit out of the actor
    sb      t2, 0x4cc(s0) ; Store it in the new space added to the actor
    andi    t2, t9, 0x3f ; Replaced code but changed to only use 0x3f
    jr      ra
    sh      t2, 0x1c(s0) ; Replaced code


