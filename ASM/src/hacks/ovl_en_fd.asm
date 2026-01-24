; Hacks in En_Fd (Flare Dancer)
.headersize(0x80954F50 - 0x00CF25E0)

; Hook EnFd_Update so we can add BgCheck
; Replace the update function in the ActorInit struct
.org 0x80957638
; Replaces
;   .dw     EnFd_Update ; Reloc 0x26e8
    .dw EnFd_Update_Hook

; Hack in EnFd_Update to not play the miniboss BGM in enemizer
; At the call to func_800C6820 which is what sets the background music
.org 0x8095639C
ENFD_PATCH_BGM_ENEMIZER_START:
; Replaces:
;   jal     func_800C6820
    nop
ENFD_PATCH_BGM_ENEMIZER_END:

; Relocs
.org 0x80957bf4
nop
