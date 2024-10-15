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

; Hack EnKusa_Draw function so it can match contents
; Replace when actor.draw is set in EnKusa_WaitObject to our new function
.org 0x80a80150
; Replaces
;   lui     t8, 0x80a8
;   li      at, -0x11
;   addiu   t8, t8, 0xa50
    lui      t8, hi(EnKusa_Draw_Hack)
    li      at, -0x11
    addiu   t8, t8, lo(EnKusa_Draw_Hack)

; Remove the address from the overlay table
.org 0x80a80c4c
    nop
    nop

; Move EnKusa up a little bit
.org 0x80a7f8d8
; Replaces
;   lui     at, 0x41f0
;    lui     at, 0x4348


; Drawing hacks
; Increase the size of En_Kusa
.org 0x80a80adc
.word  0x000001A0 ; Replaces 0x00000190

; Hack bigger grass drawing
.orga 0xF5F000 + 0xb9d0 + 0x18 ; gameplay_field_keep file start + dlist offset + gDPSetTextureImage offset
.word   0xDE000000, 0x09000000 ; jump to the custom dlist at segment 09
.orga 0xF5F000 + 0xb9d0 + 0x70 ; gameplay_field_keep file start + dlist offset + gDPSetPrimColor offset
.word   0x00000000, 0x00000000 ; nop because we'll set primcolor in our function

; Hack small grass drawing
; object_kusa_DL_000140
.orga 0x01737000 + 0x140 + 0x18 ; object_kusa start + dlist offset + gDPSetTextureImage offset
.word   0xDE000000, 0x09000000 ; jump to the custom dlist at segment 09
.orga 0x01737000 + 0x140 + 0x70 ; object_kusa start + dlist offset + gDPSetPrimColor offset
.word   0x00000000, 0x00000000 ; nop because we'll set primcolor in our function
