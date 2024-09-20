.headersize(0x808B3FC0 - 0xC51860)

; Don't draw debris
.org 0x808B5FC0
; Replaces:
;   jal     Actor_SpawnAsChild
    nop
