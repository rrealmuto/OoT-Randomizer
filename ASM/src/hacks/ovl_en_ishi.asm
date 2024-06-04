.headersize (0x80AB2EC0 - 0x00E32D10)

; Hack the actor initialization variable to use the new standalone object
.org 0x80AB4420
dh  0x01B5

; Hack the params of the draw function to pass in the segment offset of our new display list
.org 0x80ab43a8
; Replaces 
;   lui     t5, 0x0501
;   addiu   t5, t5, -0x5C48
    lui     t5, 0x0600
    addiu   t5, t5, 0x09E0

; Hack the EffectSsKakera_Spawn params to use the new standalone object
.org 0x80ab32c8
; Replaces
;   lui     s6, 0x0501
;   mtc1    at, f24
;   li      s1, 0x1000
;   addiu   s6, s6, -0x5a18
    lui     s6, 0x0600
    .skip   8
    addiu   s6, s6, 0x0C10

.org 0x80ab34b8
; Replaces
;   li      t5, 0x2
    li      t5, 0x1B5

; Make it a bit easier to pick up en_ishi when they're resized
.org 0x80ab3cdc
; Replaces:
;   bc1fl   0x801f6894
;   lw      ra, 0x24(sp)
;   bne     t2, at, 0x80ab3d14
;   or      a0, s0, r0
;   lui     at, 0x41a0
;   mtc     at, f4
;   or      a0, s0, r0
;   or      a1, s1, r0
;   or      a2, r0, r0
;   lui     a3, 0x42a0
;   jal     0x80022BD4
;   swc1    f4, 0x0010(sp)
;   b       0x80ab3d34
;   lw      ra, 0x0024(sp)
;   lui     at, 0x4120
;   mtc1    at, f6
;   or      a1, s1, r0
;   or      a2, r0, r0
;   lui     a3, 0x4248
;   jal     0x80022BD4
;   swc1    f6, 0x0010(sp)
; Remove the code that checks < 90.0 because it's unnecessary as it gets checked in the call to offer_getitem
    bne     t2, at, 0x80ab3d14
    or      a0, s0, r0
    nop
    nop
.org 0x80ab3d04
; Hook call to Actor_GetItem
; Replaces:
;   jal     0x80022BD4
    jal     EnIshi_OfferGetItem_Hook