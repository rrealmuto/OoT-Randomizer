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

; Hack in EnFd_Draw to not call EnFd_DrawEffectsDots
; This shit fills up the DLs way too much seems to be the main cause of many reported crashes
.org 0x80956954
ENFD_PATCH_DRAW_DOTS_START:
; Replaces:
;   jal     EnFd_DrawEffectsDots ; Reloc'd 0x1A04
    nop
ENFD_PATCH_DRAW_DOTS_END:

; And don't call EnFd_UpdateEffectsDots since we're not going to draw them...
.org 0x809564ac
ENFD_PATCH_UPDATE_DOTS_START:
; Replaces:
;   jal     EnFd_UpdateEffectsDots ; Reloc'd 0x155C
    nop
ENFD_PATCH_UPDATE_DOTS_END:

; Relocs
.org 0x80957bf4
nop
.org 0x80957ba4
nop
.org 0x80957b18
nop