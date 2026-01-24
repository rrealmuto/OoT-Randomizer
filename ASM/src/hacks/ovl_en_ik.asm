; Hacks in ovl_en_ik (Iron Knuckle)
.headersize(0x80A66F40 - 0xDE98A0)

; Increase the size of EnIk instance
.org 0x80a6b094
.word 0x000004dc ; was 0x4cc

; Hack in EnIk_InitImpl when storing params to handle additional param data
; We will store an additional bit at 0x0080 to indicate whether black/white knuckles should attack immediately
.org 0x80a67088
; Replaces
;   lh      t9, 0x1c(s0)  *
;   lh      t6, 0x1c(s0)
;   lui     t3, 0x80a7 ; (reloc so make sure this stays in the same spot)
;   addiu   t3, t3, -0x515c (reloc so make sure this stays in the same spot)
;   andi    t2, t9, 0xff  *
;   sw      t3, 0x98(s0)
;   sh      t2, 0x1c(s0)  *
    jal     EnIk_Params_Hack
    lh      t9, 0x1c(s0)
    lui     t3, 0x80a7
    addiu   t3, t3, -0x515c
    or      t6, t9, r0 ; Same as    lh t6, 0x1c(s0) because t9 should still contain original params
    sw      t3, 0x98(s0)
    nop

; Hack in EnIk_InitImpl (non-nabooru knuckle) to immediately attack the player
.org 0x80a671a0
; Replaces:
;   jal     OVL_EnIk_SetupStandUp
    jal     EnIk_InitSetup_Hack ; (Reloc 0x260)

; Fix relocs
.org 0x80a6b194 ;(0x260)
    nop
