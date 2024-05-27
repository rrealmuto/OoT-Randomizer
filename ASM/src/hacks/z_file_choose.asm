; File select screen

.headersize(0x80803880 - 0x00BA12C0)

; Remove loading the game when pressing OK on the file screen.
; This is managed manually in file_select.c instead.

; Validation sound effect
.org 0x80810C98
; Replaces jal     func_800C806C
    nop

; Load the game
.org 0x80810CA8
; Replaces:
;    addiu   t1, $zero, 0x0006
    addiu   t1, $zero, 0x0003

; Stop the menu music
.org 0x80810CB0
; Replaces:
;    jal     func_800C7684
    nop