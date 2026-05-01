; Hacks in Arms_Hook (hookshot actor that gets spawned when you pull out the hookshot)
.headersize(0x8090FB40 - 0xCAD2C0)

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

; Hacks in code file to fix reticle
.headersize (0x800110A0 - 0xA87000)
.org 0x8007b70c
    nop
    nop
    nop
    nop