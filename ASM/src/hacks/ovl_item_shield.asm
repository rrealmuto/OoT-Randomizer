; Hack in Item_Shield
.headersize(0x80A28410-0x00DB1F40)

; Hack in ItemShield_Draw to properly set segment 0x0C so the game doesn't crash when the shield burns
.org 0x80A28BB4
; Replaces:
;   jal     Gfx_SetupDL_25Opa ; (0x8007e298)
    jal     ItemShield_Draw_Setup