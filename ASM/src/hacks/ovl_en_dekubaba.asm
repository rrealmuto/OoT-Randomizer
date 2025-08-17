; Hacks in en_dekebaba (Deku Baba)

.headersize(0x808FB480 - 0xC98C40)

; Don't ever go into the waiting state
; In EnDekubaba_SetupWait go directly into the EnDekuBaba_SetupGrow
.org 0x808fb990
; Replaces:
;   lui     t9, 0x8090
;   addiu   t9, t9, -0x3e50
    EN_DEKUBABA_PATCH1_ENEMIZER_START:
    li      t9, OVL_EnDekubaba_SetupGrow
    EN_DEKUBABA_PATCH1_ENEMIZER_END:

; Always go into the lunge state after growing

.org 0x808fc6a8
; Replaces:
;   jal     EnDekubaba_SetupRetract
    EN_DEKUBABA_PATCH2_ENEMIZER_START:
    jal     OVL_EnDekubaba_SetupPrepareLunge
    EN_DEKUBABA_PATCH2_ENEMIZER_END:


; In EnDekubaba_DecideLunge never retract
.org 0x808fcd68
; Replaces:
;   jal     EnDekubaba_SetupRetract
    EN_DEKUBABA_PATCH3_ENEMIZER_START:
    jal     OVL_EnDekubaba_SetupPrepareLunge
    EN_DEKUBABA_PATCH3_ENEMIZER_END:
