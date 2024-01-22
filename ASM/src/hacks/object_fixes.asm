; Fixes in overlays that would otherwise be broken by our extended object system
; Found these by looking up all instances of 0x17B4(xx) in disasm
; Should hopefully be all of them

; En_Door
; Fix EnDoor which tries to use GAMEPLAY_FIELD_KEEP for doors in dungeons that don't have their own door objects
.headersize(0x80867920 - 0xC05200)
; hack both calls to Object_GetIndex in EnDoor_Init so they bypass our hack. Doors should never be loaded into extended space
.org 0x808679c8
jal Object_GetIndex
.org 0x808679f4
jal Object_GetIndex

; Door_Killer
; Fix function to load the correct texture which indexes the slots directly
.headersize(0x80B74270 - 0x00EEF990)
.org 0x80B752EC
; Replaces:
; -  lui     a2, 0x0001
;    lui     a3, 0x8000
;    lbu     t6, 0x020C(a0)
;    lui     at, 0x00FF
;    ori     at, at, 0xFFFF
;  -  sll     t7, t6,  4
;  -  addu    t7, t7, t6
;  -  sll     t7, t7,  2
;  -  addu    t8, a1, t7
;  -  addu    t9, t8, a2
;  -  lw      t0, 0x17B4(t9)
; t6 contains slot
; put address in t0
    lui     a3, 0x8000
    lbu     t6, 0x020C(a0)
    lui     at, 0x00FF
    ori     at, at, 0xFFFF
    li      t0, extended_object_ctx ; takes 2 instruction slots
    addiu   t0, t0, 0x14
    sll     t7, t6, 0x03
    addu    t0, t0, t7
    lw      t0, 0x0(t0)
    nop

; the second time around
.org 0x80b75340
;  t0 contains the slot
;  put address in t4
; Replaces:
;  -   sll     t1, t0,  4
;  -   addu    t1, t1, t0
;     lui     $at, 0x8000
;  -   sll     t1, t1,  2
;     addu    t8, t6, t7
;     addu    t9, t8, $at
;  -   addu    t2, a1, t1
;     sw      t9, 0x0204(a0)             # 00000204
;  -   addu    t3, t2, a2
;  -   lw      t4, 0x17B4(t3)             # 000017B4
    lui     at, 0x8000
    addu    t8, t6, t7
    addu    t9, t8, at
    sw      t9, 0x0204(a0)
    li      t4, extended_object_ctx ; takes 2 instruction slots
    addiu   t4, t4, 0x14
    sll     t1, t0, 0x03
    addu    t4, t4, t1
    lw      t4, 0x0(t4)

; Hack the call the Object_GetIndex in DoorKiller_Init to just call the vanilla function
; So that the proper fake door object loads
.org 0x80b742b0 
; Replaces:
;   jal Object_GetIndex ; (original, which is hacked)
    jal Object_GetIndex ; (our version of the original, bypassing the hack)

; ovl_effect_ss_kakera (fragment ss effect used by boulders/pots/etc)
; Hack where it indexes the object table directly
; First for XLU
.headersize(0x80B34AB0 - 0xEB01E0)
.org 0x80B34D04
; Replaces:
;    lui        t8,0x1
;    sll        t6,t5,0x4
;    addu       t6,t6,t5
;    sll        t6,t6,0x2
;    addu       t7,a0,t6
;    addu       t8,t8,t7
;    lw         t8, 0x17b4(t8)
;   t5 currently contains the slot index
;   we want t8 to contain the address for the slot
    li      t8, extended_object_ctx ; takes 2 instruction slots
    addiu   t8, t8, 0x14
    sll     t6, t5, 0x03
    addu    t8, t8, t6
    lw      t8, 0x0(t8)
    nop
; second time for OPA
.org 0x80B34D48
; Replaces:
;   lui        t4,0x1
;   sll        t2,t1,0x4
;   addu       t2,t2,t1
;   sll        t2,t2,0x2
;   addu       t3,a0,t2
;   addu       t4,t4,t3
;   lw         t4,0x17b4(t4)
;   t1 contains the slot index
;   we want t4 to contain the address for the slot
    li      t4, extended_object_ctx ; takes 2 instruction slots
    addiu   t4, t4, 0x14
    sll     t2, t1, 0x03
    addu    t4, t4, t2
    lw      t4, 0x0(t4)
    nop

; ovl_effect_ss_hahen (Burst effect, used by octo shot and breaks if octo is spawned on extended objects)
; t0 contains the slot index
; we want t3 to contain the address for the slot
.headersize(0x80B31380 - 0x00EACAB0)
.org 0x80B315F8
; Replaces:
;   lui     t3, 0x0001
;   sll     t1, t0,  4
;   addu    t1, t1, t0
;   sll     t1, t1,  2
;   addu    t2, a0, t1
;   addu    t3, t3, t2
;   lw      t3, 0x17B4(t3)
    li      t3, extended_object_ctx ; takes 2 instruction slots
    addiu   t3, t3, 0x14
    sll     t1, t0, 0x03
    addu    t3, t3, t1
    lw      t3, 0x0(t3)
    nop

.org 0x80B31758
; Replaces:
;   lui     t3, 0x0001
;   sll     t1, t0,  4
;   addu    t1, t1, t0
;   sll     t1, t1,  2
;   addu    t2, a0, t1
;   addu    t3, t3, t2
;   lw      t3, 0x17B4(t3)
    li      t3, extended_object_ctx ; takes 2 instruction slots
    addiu   t3, t3, 0x14
    sll     t1, t0, 0x03
    addu    t3, t3, t1
    lw      t3, 0x0(t3)
    nop

; effect_ss_d_fire
.headersize(0x80B2EEB0 - 0x00EAA5E0)
.org 0x80B2F01C
; Replaces:
;  - lui     t0, 0x0001                 # t0 = 00010000
;   lw      s0, 0x0000(a3)             # 00000000
;  - sll     t7, t6,  4
;  - addu    t7, t7, t6
;  - sll     t7, t7,  2
;  - addu    t8, a3, t7
;  - addu    t0, t0, t8
;  - lw      t0, 0x17B4(t0)             # 000117B4
;   index in t6
;   addr goes in t0
    lw      s0, 0x0000(a3) ; replaced code not part of the hack
    li      t0, extended_object_ctx ; takes 2 instruction slots
    addiu   t0, t0, 0x14
    sll     t7, t6, 0x03
    addu    t0, t0, t7
    lw      t0, 0x0(t0)
    nop

; effect_ss_extra
.headersize(0x80B375D0 - 0x00EB2D00)
.org 0x80B37644
; Replaces:
;    sll     t7, a1,  4
;    addu    t7, t7, a1
;    sll     t7, t7,  2
;    lui     t9, 0x0001                 # t9 = 00010000
;    addu    t8, t6, t7
;    addu    t9, t9, t8
;    lw      t9, 0x17B4(t9)             # 000117B4
;   index in a1
;   addr goes in t9
    li      t9, extended_object_ctx ; takes 2 instruction slots
    addiu   t9, t9, 0x14
    sll     t7, a1, 0x03
    addu    t9, t9, t7
    lw      t9, 0x0(t9)
    nop

.org 0x80B37744
; Replaces:
;  -  lui     v0, 0x0001                 # v0 = 00010000
;    lui     $at, 0x8000                # $at = 80000000
;    cvt.s.w $f6, $f4
;    or      a3, $zero, $zero           # a3 = 00000000
;    div.s   $f10, $f6, $f8
;    swc1    $f10, 0x0040($sp)
;    lh      t7, 0x0040(s2)             # 00000040
;  -  sll     t8, t7,  4
;  -  addu    t8, t8, t7
;  -  sll     t8, t8,  2
;  -  addu    t9, s1, t8
;  -  addu    v0, v0, t9
;  -  lw      v0, 0x17B4(v0)             # 000117B4
     lui     at, 0x8000
     cvt.s.w f6, f4
     or      a3, r0, r0
     div.s   f10, f6, f8
     swc1    f10, 0x0040(sp)
     lh      t7, 0x0040(s2)
;   index in t7
;   addr goes in v0
    li      v0, extended_object_ctx ; takes 2 instruction slots
    addiu   v0, v0, 0x14
    sll     t8, t7, 0x03
    addu    v0, v0, t8
    lw      v0, 0x0(v0)
    nop

; effect_ss_fhg_flash (phantom ganon light ball)
.headersize(0x80B33550 - 0x00EAEC80)
.org 0x80B335FC
; Replaces:
;    sll     t5, a1,  4
;    addu    t5, t5, a1
;    sll     t5, t5,  2
;    lui     t7, 0x0001                 # t7 = 00010000
;    addu    t6, t4, t5
;    addu    t7, t7, t6
;    lw      t7, 0x17B4(t7)             # 000117B4
;   index in a1
;   addr goes in t7
    li      t7, extended_object_ctx ; takes 2 instruction slots
    addiu   t7, t7, 0x14
    sll     t5, a1, 0x03
    addu    t7, t7, t5
    lw      t7, 0x0(t7)
    nop

.org 0x80B338C8
; Replaces:
;  -  lui     t0, 0x0001                 # t0 = 00010000
;    cvt.s.w $f6, $f4
;    or      a3, $zero, $zero           # a3 = 00000000
;    div.s   $f10, $f6, $f8
;    swc1    $f10, 0x0044($sp)
;    lh      t7, 0x0044(s1)             # 00000044
;  -  sll     t8, t7,  4
;  -  addu    t8, t8, t7
;  -  sll     t8, t8,  2
;  -  addu    t9, a0, t8
;  -  addu    t0, t0, t9
;  -  lw      t0, 0x17B4(t0)             # 000117B4
    cvt.s.w f6, f4
    or      a3, r0, r0
    div.s   f10, f6, f8
    swc1    f10, 0x0044($sp)
    lh      t7, 0x0044(s1)
;   index in t7
;   addr goes in t0
    li      t0, extended_object_ctx ; takes 2 instruction slots
    addiu   t0, t0, 0x14
    sll     t8, t7, 0x03
    addu    t0, t0, t8
    lw      t0, 0x0(t0)
    nop

; effect_ss_g_magma2 (used by king dodongo)
.headersize(0x80B32760 - 0x00EADE90)
.org 0x80B327E0
; Replaces:
;  -  sll     t0, a1,  4
;  -  addu    t0, t0, a1
;    sw      t8, 0x0000(v1)             # FFFFFFEC
;    lw      t7, 0x0004(t6)             # 80B32C20
;  -  sll     t0, t0,  2
;  -  lui     t2, 0x0001                 # t2 = 00010000
;    sw      t7, 0x0004(v1)             # FFFFFFF0
;    lw      t8, 0x0008(t6)             # 80B32C24
;    lui     $at, 0x8000                # $at = 80000000
;    lui     v0, 0x0602                 # v0 = 06020000
;    sw      t8, 0x0008(v1)             # FFFFFFF4
;    lw      t9, 0x0038($sp)
;    addiu   v0, v0, 0x4690             # v0 = 06024690
;  -  addu    t1, t9, t0
;  -  addu    t2, t2, t1
;  -  lw      t2, 0x17B4(t2)
;   index in a1
;   addr goes in t2
    sw      t8, 0x0000(v1)
    lw      t7, 0x0004(t6)
    sw      t7, 0x0004(v1)
    lw      t8, 0x0008(t6)
    lui     $at, 0x8000
    lui     v0, 0x0602
    sw      t8, 0x0008(v1)
    lw      t9, 0x0038($sp)
    addiu   v0, v0, 0x4690
    li      t2, extended_object_ctx ; takes 2 instruction slots
    addiu   t2, t2, 0x14
    sll     t0, a1, 0x03
    addu    t2, t2, t0
    lw      t2, 0x0(t2)
    nop

.org 0x80B32958
; Replaces:
;  -  lui     t0, 0x0001                 # t0 = 00010000
;    cvt.s.w $f6, $f4
;    or      a3, $zero, $zero           # a3 = 00000000
;    div.s   $f10, $f6, $f8
;    swc1    $f10, 0x0034($sp)
;    lh      t7, 0x0054(s1)             # 00000054
;  -  sll     t8, t7,  4
;  -  addu    t8, t8, t7
;  -  sll     t8, t8,  2
;  -  addu    t9, a0, t8
;  -  addu    t0, t0, t9
;  -  lw      t0, 0x17B4(t0)             # 000117B4
    cvt.s.w f6, f4
    or      a3, r0, r0
    div.s   f10, f6, f8
    swc1    f10, 0x0034(sp)
    lh      t7, 0x0054(s1)
; index in t7
; addr goes in t0
    li      t0, extended_object_ctx ; takes 2 instruction slots
    addiu   t0, t0, 0x14
    sll     t8, t7, 0x03
    addu    t0, t0, t8
    lw      t0, 0x0(t0)
    nop

; effect_ss_ice_smoke (vanilla ice trap smoke from a chest)
.headersize(0x80B7E700 - 0x00EF9E20)
.org 0x80B7E750
; Replaces:
;  -  lui     t1, 0x0001                 # t1 = 00010000
;    lui     v0, 0x8012                 # v0 = 80120000
;    addiu   v0, v0, 0x0C38             # v0 = 80120C38
;    lw      t8, 0x002C(sp)
;    lw      t7, 0x0038(sp)
;    lw      t6, 0x0018(v0)             # 80120C50
;  -  sll     t9, t8,  4
;  -  addu    t9, t9, t8
;  -  sll     t9, t9,  2
;  -  addu    t0, t7, t9
;  -  addu    t1, t1, t0
;    sw      t6, 0x0028(sp)
;  -  lw      t1, 0x17B4(t1)             # 000117B4
; index in t8
; addr goes in t1
    lui     v0, 0x8012
    addiu   v0, v0, 0x0C38
    lw      t8, 0x002C(sp)
    lw      t7, 0x0038(sp)
    lw      t6, 0x0018(v0)
    sw      t6, 0x0028(sp)
    li      t1, extended_object_ctx ; takes 2 instruction slots
    addiu   t1, t1, 0x14
    sll     t9, t8, 0x03
    addu    t1, t1, t9
    lw      t1, 0x0(t1)
    nop

.org 0x80B7E844
; Replaces:
;  -  lui     t9, 0x0001                 # t9 = 00010000
;    lui     at, 0x0001                # $at = 00010000
;  -  sll     t7, t6,  4
;  -  addu    t7, t7, t6
;  -  sll     t7, t7,  2
;  -  addu    t8, s1, t7
;  -  addu    t9, t9, t8
;  -  lw      t9, 0x17B4(t9)             # 000117B4
    lui     at, 0x0001
; index in t6
; addr goes in t9
    li      t9, extended_object_ctx ; takes 2 instruction slots
    addiu   t9, t9, 0x14
    sll     t7, t6, 0x03
    addu    t9, t9, t7
    lw      t9, 0x0(t9)
    nop

; effect_ss_stick
.headersize(0x80B319C0 - 0x00EAD0F0)
.org 0x80B31C4C
; Replaces:
;    lui     t7, 0x0001                 # t7 = 00010000
;    sll     t5, t4,  4
;    addu    t5, t5, t4
;    sll     t5, t5,  2
;    addu    t6, t3, t5
;    addu    t7, t7, t6
;    lw      t7, 0x17B4(t7)             # 000117B4
; index in t4
; addr goes in t7
    li      t7, extended_object_ctx ; takes 2 instruction slots
    addiu   t7, t7, 0x14
    sll     t5, t4, 0x03
    addu    t7, t7, t5
    lw      t7, 0x0(t7)
    nop

; Hack in en_holl to prevent clearing the extended object table when transitioning into the same room.
; Replacing the call to the room clear function 0x80080c98
.headersize(0x80895700 - 0x00C32FD0)
.org 0x80895A28
; Replaces:
;   jal     0x80080c98
    jal     EnHoll_Room_Change_Hook

; Hack in Player actor overlay for dogs that can follow through loading zones
.headersize (0x808301c0 - 0xBCDB70)
; Hack the call to Object_GetIndex in Player_Update when it's checking if the dog object is loaded
.org 0x80847ffc
; Replaces:
;   jal     Object_GetIndex ; (original which jumps to our hack)
    jal     Object_GetIndex_EnDog ; (special hacked version just for doggos)

; Hacks in En_Hy to just not use the extended objects system
.headersize(0x80AE4EE0 - 0x00E641C0)
; Replace calls to Object_GetIndex in EnHy_FindSkelAndHeadObjects
.org 0x80ae4f44
    jal     Object_GetIndex
.org 0x80ae4f7c
    jal     Object_GetIndex
.org 0x80ae4fbc
    jal     Object_GetIndex
.org 0x80ae5098
    jal     Object_GetIndex