; Hacks in Arms_Hook (hookshot actor that gets spawned when you pull out the hookshot)
.headersize(0x8090FB40 - 0xCAD2C0)

; Hack in actor init var to use GAMEPLAY_KEEP object_id so it always spawns
.org 0x809106E8
    dh  0x0001

; Hack in ArmsHook_Draw to set segment to our new hookshot object
; Hack at call to Gfx_SetupDL_25Opa
.org 0x8091056c
; Replaces:
;   jal     Gfx_SetupDL_25OPA
    jal     ArmsHook_Draw_SetupHack

; Hack in ArmsHook_Draw when calling gLinkAdultHookshotTipDL to use DL in our new object
; See object_hookshot_new in Patches.py
.org 0x809105b0
; Replaces:
;   lui     t9, 0x0603
;   addiu   t9, t9, -0x4d78
    li      t9, 0x06000358

; Hack in ArmsHook_Draw when calling gLinkAdultHookshotChainDL to use DL in our new object
; See object_hookshot_new in Patches.py
.org 0x809106a4
; Replaces:
;   lui     t5, 0x0603
;   addiu   t5, t5, -0x5010
    li      t5, 0x06000180

; Hacks in code file
; Fix reticle to use reticle DL in new hookshot object
.headersize (0x800110A0 - 0xA87000)
; Hack when Player_DrawHookshotReticle sets the segment to use our new hookshot object stored persistently
; new segment address needs to go into t0
.org 0x8007B6D8
; Replaces:
;   lw      t6, 0x9C(sp)
;   lui     t0, 0x01
;   lb      t7, 0x1e(t6)
;   sll     t8, t7, 0x04
;   addu    t8, t8, t7
;   sll     t8, t8, 0x02
;   addu    t9, s1, t8
;   addu    t0, t0, t9
;   lw      t0, 0x17b4(t0)
;   sw      t0, 0x04(v1) <- keep this
    lui     t6, hi(object_hookshot_new)
    addiu   t6, lo(object_hookshot_new) + 4
    lw      t0, 0x00(t6)
    nop
    nop
    nop
    nop
    nop
    nop

; Hack DL in Player_DrawHookshotReticle to use the new reticle DL in the new object
.org 0x8007b704
; Replaces:
;   lui     t3, 0x0603
;   addiu   t3, t3, -0x34b8
    li      t3, 0x06001838

; Fix first person view arms
; Hook Player_OverrideLimbDrawGameplayFirstPerson to override what is done for the PLAYER_LIMB_R_HAND case
; We need to do this because we need the leave the original function intact for custom model patching (for now)
; If we ever decide to change the model patcher, can use the hack below instead
; Basically just replacing all references to Player_OverrideLimbDrawGameplayFirstPerson
; One in Player_DrawImpl
.org 0x8007a0dc
; Replaces:
;   lui     t8, 0x8008
;   addiu   t8, t8, -0x5280 (these 2 instructions load t8 with Player_OverrideLimbDrawGameplayFirstPerson)
    li  t8, Player_OverrideLimbDrawGameplayFirstPerson_Hook

; One in Player_Draw (in player overlay)
.headersize(0x808301C0 - 0x00BCDB70)
.org 0x808487f8
; Replaces:
;   lui     t7, 0x8008
;   addiu   t7, t7, -0x5280
    li      t7, Player_OverrideLimbDrawGameplayFirstPerson_Hook

; Hack in Player_OverrideLimbDrawGameplayFirstPerson when assigning *dList when holding hookshot (limbIndex == PLAYER_LIMB_R_HAND) && Player_HoldsHookshot
; Override the call to Player_HoldsHookshot and remove the assignment code after it, do everything in the new function
; New function will just return the DL
; DList pointer is a2
;.org 0x8007ae78
; Replaces:
;   jal     Player_HoldsHookshot
;   sw      a2, 0x28(sp)
;   beq     v0, r0, 0x8007AE98
;   lw      a2, 0x28(sp)
;   lui     t7, 0x0603
;   addiu   t7, t7, -0x58C8
;   b       0x8007AEBC
;   sw      t7, 0x00(a2)
;   lui     v0, 0x8012
;   lw      v0, -0x5a2c(v0)=>DAT_8011a5d4
;   lui     t9, 0x800f
;   sll     t8, v0, 0x2
;   addu    t9, t9, t8
;   lw      t9, offset DAT_800f7a08(t9)                       = 06h
;   b       LAB_8007aebc
;   _sw     t9,0x0(a2)
;    jal     Player_GetRightHandFirstPersonDL
;    sw      a2, 0x28(sp) ; Replaced code
;    b       0x8007AEBC ; Branch to where the original code would exit
;    sw      v0, 0x00(a2) ; New function returns the DL in v0 so just store it into the DList pointer in a2
