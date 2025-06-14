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
.org 0x808265AC
; Replaces:
;   jal     Player_InitPauseDrawData
    jal     KaleidoScope_Player_InitPauseDrawData_Hook

; Hack in KaleidoScope_Update init state to allocate space on the zeldaarena heap instead of object heap
; Hack every DmaMgr_RequestSync call in that part of the code to do a malloc instead
; Pass in pointers to the segments that we want to set instead
; s0 has pauseCtx
; Hack call to DmaMgr_RequestSync for pauseCtx->iconItemSegment
; put address of pauseCtx->iconItemSegment into a0 instead of setting it
.org 0x808265d8
; Replaces:
;   sw      a0, 0x128(s0)
;   subu    a2, t8, a1
;   jal     DmaMgr_RequestSync
    addiu   a0, s0, 0x128
    subu    a2, t8, a1
    jal     KaleidoScope_AllocAndDmaRequest

; Hack call to DmaMgr_RequestSync for pauseCtx->iconItem24Segment
; put address of pauseCtx->iconItem24Segment into a0 instead of setting it
.org 0x808266BC
; Replaces:
;   sw      a0, 0x12c(s0)
;   subu    a3, t9, a1
;   or      a2, a3, r0
;   jal     DmaMgr_RequestSync
    addiu   a0, s0, 0x12c
    subu    a3, t9, a1
    or      a2, a3, r0
    jal     KaleidoScope_AllocAndDmaRequest

; Hack calls to DmaMgr_RequestSync for pauseCtx->iconItemAltSegment
; put address of pauseCtx->iconItemAltSegment into a0 instead of setting it
.org 0x8082673C
; Replaces:
;   jal     DmaMgr_RequestSync
;   lw      a0, 0x130(s0)
    jal     KaleidoScope_AllocAndDmaRequest
    addiu   a0, s0, 0x130
.org 0x80826794
; Replaces:
;   jal     DmaMgr_RequestSync
;   lw      a0, 0x130(s0)
    jal     KaleidoScope_AllocAndDmaRequest
    addiu   a0, s0, 0x130

; Hack calls to DmaMgr_RequestSync for pauseCtx->iconItemLangSegment
; put address of pauseCtx->iconItemLangSegment into a0 instead of setting it
.org 0x808267e0
; Replaces:
;   jal     DmaMgr_RequestSync
;   lw      a0, 0x134(s0)
    jal     KaleidoScope_AllocAndDmaRequest
    addiu   a0, s0, 0x134
.org 0x80826808
; Replaces:
;   jal     DmaMgr_RequestSync
;   lw      a0, 0x134(s0)
    jal     KaleidoScope_AllocAndDmaRequest
    addiu   a0, s0, 0x134

; Hack calls to DmaMgr_RequestSync for pauseCtx->nameSegment
; put address of pauseCtx->nameSegment into a0 instead of setting it
; This one kinda stupid because it offsets by 0x400 to put the map
; In KaleidoScope_UpdateNamePanel it DMAs the first 0x400
; So what we want to do is allocate the full space for nameSegment here. We'll make this its own function
; Then the stuff in KaleidoScope_UpdateNamePanel can stay as is
.org 0x80826890
; Replaces:
;   jal     DmaMgr_RequestSync
;   addiu   a0, a0, 0x400
    jal     KaleidoScope_AllocAndDmaRequestNameSegment
    addiu   a0, s0, 0x138
.org 0x808268C8
; Replaces:
;   jal     DmaMgr_RequestSync
;   addiu   a0, a0, 0x400
    jal     KaleidoScope_AllocAndDmaRequestNameSegment
    addiu   a0, s0, 0x138

; Hack call to PreRender_SetValuesSave so we can allocate space for sPreRenderCvg
; pass pointer to sPreRengerCvg in 0x14(sp)
; this is stored in the overlay so we need to relocate it.
; VRAM address 0x808301a8
.org 0x808268f4
; Replaces:
;   lui     t7, 0x8083
;   lw      t7, 0x01a8(t7)
    lui     t7, 0x8083
    addiu   t7, 0x01a8
.org 0x80826914
; Replaces:
;   jal     PreRender_SetValuesSave
    jal     KaleidoScope_PreRender_SetValuesSave_Hook

; Hook the calls to the function that reloads all of the objects
; So that we can free anything that we allocated for KaleidoScope
; Function is named func_800981B8 in decomp and it's located at 800816b8
; I'll call it Object_ReloadSlots

.org 0x80828828
; Replaces:
;   addu    a0, s1, at
;   jal     Object_ReloadSlots
    or      a0, s1, r0 ; Pass play instead of objectctx
    jal     KaleidoScope_FreeMemAndReloadSlots

.org 0x80828b24
; Replaces:
;   addu    a0, s1, at
;   jal     Object_ReloadSlots
    or      a0, s1, r0 ; Pass play instead of objectctx
    jal     KaleidoScope_FreeMemAndReloadSlots

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