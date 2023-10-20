; Hacks for Grass/Bushes
.headersize(0x80A7F8B0 - 0xE021F0) 

; Hack EnKusa_DropCollectible to override drops for grass shuffle
; At the start of EnKusa_DropCollectible
.org 0x80a7f964
; Replaces
;   addiu   sp, sp, -0x18
;   sw      ra, 0x14(sp)
;   sw      a0, 0x18(sp)
;   sw      a1, 0x1c(sp)
    addiu   sp, sp, -0x18
    sw      ra, 0x10(sp)
    jal     enkusa_dropcollectible_hook
    nop

; Move EnKusa up a little bit
.org 0x80a7f8d8
; Replaces
;   lui     at, 0x41f0
;    lui     at, 0x4348