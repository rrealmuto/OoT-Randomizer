; Reentry point for original function
Object_IsLoaded:
    sll     t6, a1, 0x4
    j       Object_IsLoaded_After
    addu    t6, t6, a1

; Reentry point for original function
Actor_SetObjectDependency:
    lb      t6, 0x1e(a1)
    j       Actor_SetObjectDependency_After
    lui     t9, 0x01

; Reentry point for original function
Object_GetIndex:
    sll     a3, a1, 0x10
    j       Object_GetIndex_After
    sra     a3, a3, 0x10

; Reentry point for original function
Scene_CommandObjectList:
    addiu   sp, sp, -0x40
    j       Scene_CommandObjectList_After
    sw      s8, 0x38(sp)

; Reentry point for original function
Room_Change:
    addiu   sp, sp, -0x28
    j       Room_Change_Continue
    sw      s0, 0x18(sp)

; Code to update the extended object table with the vanilla entries
; t4 contains the object id that has been made positive
; s2 contains the loop index (the slot)
; store it into our extended table
; we can use anything below t4 i think
Object_UpdateEntries_Extended:
    li      t3, extended_object_ctx
    addiu   t3, t3, 0x10
    sll     t2, s2, 3
    add     t3, t2, t3
    sh      t4, 0x0(t3)
    sh      t4, 0x0(s0) ; Replaced code
    jr      ra
    lbu        t5,0x8(s4) ; Replaced code

; Hooking calling to unload room from EnHoll so we can pass the holl actor
; actor is in a3
EnHoll_Room_Change_Hook:
    j   EnHoll_Room_Change_Hack
    or  a2, r0, a3
