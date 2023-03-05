; Change EnItem00 proximity check when it's an overridden silver rupee

; Actor stored in s0
; globalctx stored in s1
EnItem00_ProximityCheck_Hook:
; Set up args and call our hack
    or      a0, s0, r0
    jal     EnItem00_ProximityCheck_Hack
    or      a1, s1, r0
; Check our return result in v0. If it's true (actor is in proximity) then continue in the function, otherwise return
    bnez    v0, @continue_outside
    lw      ra, 0x0024(sp)
@continue_inside:
    j       0x80012C64
    nop

@continue_outside:
    j       0x80012C78
    lui     t6, 0x0001
;    lw      s0, 0x1C(sp)
;    lw      s1, 0x20(sp)
;    jr      ra
;    addiu   sp, sp, 0x48