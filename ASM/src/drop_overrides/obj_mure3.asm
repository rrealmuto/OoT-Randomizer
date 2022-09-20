; Hacks for rupee towers to drop flagged collectibles

; Add additional params to Item_DropCollectible2 params argument in a2
; obj_mure3 pointer should be in s2
; index of the for loop is in s0
obj_mure3_hack:
    lh      a3, 0x18(s2)   ; get our new flag out of the z rotation
    beqz    a3, obj_mure3_hack_end
    or      a2, r0, r0
    add     a3, s0 ; add our loop index
    ; get the lower 0x3F bits and put them in the regular spot in params
    andi    a1, a3, 0x3F
    sll     a1, a1, 0x08
    or      a2, r0, a1 ; put the lower part of the flag in a2
    ; get the upper 0xC0 bits and put them in the extra space in params
    andi    a1, a3, 0xC0
    or      a2, a2, a1
obj_mure3_hack_end:
    ori     a2, a2, 0x4000
    jr      ra
    or      a1, s6, r0 ; first line of replaced code

; S0 should still have our loop index and it should be 6 when we call here
obj_mure3_redrupee_hack:
    lh      a3, 0x18(s2) ; get our new flag out of the z rotation
    beqz    a3, obj_mure3_redrupee_hack_end
    or      a2, r0, r0
    add     a3, s0 ; add the loop index
    ; get the lower 0x3F bits and put them in the regular spot in params
    andi    a1, a3, 0x3F
    sll     a1, a1, 0x08
    or      a2, r0, a1 ; put the lower part of the flag in a2
    ; get the upper 0xC0 bits and put them in the extra space in params
    andi    a1, a3, 0xC0
    or      a2, a2, a1
obj_mure3_redrupee_hack_end:
    or      a1, r0, s6 ; we used a1 so put the contents back into it.
    jr      ra
    ori     a2, a2, 0x4002
