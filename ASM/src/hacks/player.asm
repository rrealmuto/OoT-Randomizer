.headersize (0x808301c0 - 0xbcdb70)

;==================================================================================================
; Make Bunny Hood like Majora's Mask
;==================================================================================================

; Replaces: mfc1    a1, f12
;           mtc1    t7, f4
.org 0x8083c054
    jal bunny_hood
    nop

;==================================================================================================
; Prevent Mask de-equip if not on a C-button
;==================================================================================================
; In Player_ProcessItemButtons, after checking if the mask is on a C button, check if we have the mask in the trade slot or in the extended trade flags
.org 0x80831F1C
    jal     mask_check_trade_slot   ; sb      zero, 0x014F(t0)
    ;nop

; Hook calls to Player_DrawGameplay function
.org 0x808489c8
    jal Player_DrawGameplay_Hook

.org 0x80848a28
    jal Player_DrawGameplay_Hook

; nop dl call
.org 0x808483c8
    sw  r0, 0x0(v0)

.org 0x808483dc
    sw r0, 0x04(v0)

; Relocs
.org 0x808556b4
nop
.org 0x808556b8
nop