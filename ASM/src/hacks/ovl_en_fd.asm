; Hacks in En_Fd (Flare Dancer)
.headersize(0x80954F50 - 0x00CF25E0)

; Hook EnFd_Update so we can add BgCheck
; Replace the update function in the ActorInit struct
.org 0x80957638
; Replaces
;   .dw     EnFd_Update ; Reloc 0x26e8
    .dw EnFd_Update_Hook

; Relocs
.org 0x80957bf4
nop
