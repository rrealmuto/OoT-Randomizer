; KaleidoScope Overlay (Menu) hacks
; ovl_kaleido_scope:
;   VRAM Start - 0x808137C0
;   VROM Start - 0xBB11E0
;   Size - 0x1C990

; Hack calls to KaleidoScope_DrawWorldMap and KaleidoScope_DrawDungeonMap

.headersize(0x808137C0 - 0xBB11E0)

.org 0x80820850 ; offset 0xD090
;   Replaces
;   jal KaleidoScope_DrawWorldMap
    jal KaleidoScope_DrawWorldMap_CallHook

.org 0x80820b48 ; offset 0xD388
;   Replaces
;   jal KaleidoScope_DrawWorldMap
    jal KaleidoScope_DrawWorldMap_CallHook

.org 0x808207E4 ; offset 0xD024
;   Replaces
;   jal KaleidoScope_DrawDungeonMap
    jal KaleidoScope_DrawDungeonMap_CallHook

.org 0x80820AC4 ; offset 0xD304
;   Replaces
;   jal KaleidoScope_DrawDungeonMap
    jal KaleidoScope_DrawDungeonMap_CallHook

; Hack calls to PauseMapMark_Draw because for some reason it isn't part of DrawDungeonMap...
.org 0x80820840 ; offset 0xD080
    jal PauseMapMark_Draw_CallHook

.org 0x80820b38 ; offset 0xD378
    jal PauseMapMark_Draw_CallHook


; Hack in KaleidoScope_Update to free the objects that are spawned on the heap
; Just hook the call to Player_InitPauseDrawData and free everything there
;.org 0x808265AC
; Replaces:
;   jal     Player_InitPauseDrawData
;    jal     KaleidoScope_Player_InitPauseDrawData_Hook

; Hook the calls to the function that reloads all of the objects
; So that we can free anything that we allocated for KaleidoScope
; Function is named func_800981B8 in decomp and it's located at 800816b8
; I'll call it Object_ReloadSlots

.org 0x80828828
; Replaces:
;   addu    a0, s1, at
;   jal     Object_ReloadSlots
    or      a0, s1, r0 ; Pass play instead of objectctx
    jal     KaleidoScope_ReloadObjects

.org 0x80828b24
; Replaces:
;   addu    a0, s1, at
;   jal     Object_ReloadSlots
    or      a0, s1, r0 ; Pass play instead of objectctx
    jal     KaleidoScope_ReloadObjects

; Relocs
; 0xD090
.org 0x8082f62c
    nop
; 0xD388
.org 0x8082f698
    nop
; 0xD024
.org 0x8082f624
    nop
; 0xD304
.org 0x8082f68c
    nop
; 0xD080
.org 0x8082f628
    nop
; 0xD378
.org 0x8082f694
    nop