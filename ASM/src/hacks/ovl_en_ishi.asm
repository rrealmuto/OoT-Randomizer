.headersize (0x80AB2EC0 - 0x00E32D10)

; Hack the actor initialization variable to use the new standalone object
.org 0x80AB4420
dh  0x01AB

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
    li      t5, 0x1AB