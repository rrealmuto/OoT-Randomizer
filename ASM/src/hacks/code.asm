.headersize (0x800110A0 - 0xA87000)

; ==================================
; Hook Play_Init
; ==================================
.org 0x8009a750
; Replaces
;   addiu   sp, sp, -0x90
;   sw      s2, 0x28(sp)
    j   Play_Init_Hook
    nop
Play_Init_Hook_Continue:

; Hook at the start collider_setcylinder so we can override cylinder radii
.org 0x8004acec
; Replaces:
;   addiu   sp, sp, -0x18
;   sw      ra, 0x14(sp)
    j   Collider_SetCylinder_Hook
    nop
Collider_SetCylinder_AfterHook:


; =================================
; Extended objects for actors
; =================================
; Hook at the beginning of Object_IsLoaded so we can handle the extended table
.org 0x80081688
; Replaces:
; sll   t6, a1, 0x4
; addu  t6, t6, a1
    j   Object_IsLoaded_Hook
    nop
Object_IsLoaded_After:

; Hook at the beginning of Actor_SetObjectDependency so we can handle the extended table
.org 0x80020fa4
; Replaces:
;   lb  t6, 0x1e(1)
;   lui t9, 0x01
    j   Actor_SetObjectDependency_Hook
    nop
Actor_SetObjectDependency_After:

; Hook in the middle of Actor_Draw so we can set the segment using the extended table
.org 0x8002439C ; through 0x8002440c?
    addiu   sp, sp, -0x40
    sw  a0, 0x10(sp)
    sw  a1, 0x14(sp)
    sw  a2, 0x18(sp)
    sw  a3, 0x1C(sp)
    sw  v0, 0x20(sp)
    sw  v1, 0x24(sp)
    jal Actor_Draw_gSPSegment_Hack
    or  a0, s0, r0 ; Actor is in s0, move it to a0
    lw  a0, 0x10(sp)
    lw  a1, 0x14(sp)
    lw  a2, 0x18(sp)
    lw  a3, 0x1C(sp)
    lw  v0, 0x20(sp)
    lw  v1, 0x24(sp)
    addiu   sp, sp, 0x40
    lui     t7, 0x800f
    addiu   t7, t7, -0x7cd8
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

; Hook Object_GetIndex so we can check our extended table
.org 0x80081628
; Replaces
;    sll     a3, a1, 0x10
;    sra     a3, a3, 0x10
    j   Object_GetIndex_Hook
    nop
Object_GetIndex_After:

; Hook Scene_CommandObjectList to add entries to the extended table when loading scene objects (our shadow table)
.org 0x80081cd8
; Replaces
;   addiu   sp, sp, -0x40
;   sw      s8, 0x38(sp)
    j   Scene_CommandObjectList_Hook
    nop
Scene_CommandObjectList_After:

; Hack in Object_UpdateEntries to update the shadow table when an object has actually finished loading
.org 0x800815EC
; Replaces
;       sh         t4,0x0(s0)
;       lbu        t5,0x8(s4)
jal     Object_UpdateEntries_Extended
nop

; Hack in the function that i think is what changes rooms to reset the extended table on room change
.org 0x80080C98
; Replaces:
;   addiu   sp, sp, -0x28
;   sw      s0, 0x18(sp)
    j       Room_Change_Hook
    nop
Room_Change_Continue:

; Hack in Actor_UpdateAll when calling actor->update()
.org 0x800240d8
; Replaces
;   jalr    t9
;   nop
    jal     Actor_Update_Hook ; Call our hook
    or      a2, r0, t9  ; Copy the function pointer in a2 so we can call it

; Always show the debugger
.org 0x800af360
; Replaces:
;   jal     0x800ae6bc
    nop

.org 0x800af64c
; Replaces:
;   jal     0x800ae6bc
    nop