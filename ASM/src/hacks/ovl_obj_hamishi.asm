.headersize (0x80B804A0 - 0x00EFBBC0)

; Hack the actor initialization variable to use the new standalone object
.org 0x80b80c18
dh  0x1B5

; Hack the params of the draw function to pass in the segment offset of our new display list
.org 0x80b80be0
; Replaces
;   lui     t5, 0x0501
;   addiu   t5, t5, -0x5C48
    lui     t5, 0x0600
    addiu   t5, t5, 0x09E0

; Hack the EffectSsKakera_Spawn params to use the new standalone object
.org 0x80b806e0
; Replaces
;   lui     s8, 0x0501
;   mtc1    at, f24
;   li      s0, 0x0x03e8
;   addiu   s8, s8, -0x5a18
    lui     s8, 0x0600
    .skip   8
    addiu   s8, s8, 0x0C10

.org 0x80b80830
; Replaces
;   li      t2, 0x2
    li      t2, 0x1B5
