; Hacks in en_dekebaba (Deku Baba)

.headersize(0x808FB480 - 0xC98C40)

; Don't ever go into the waiting state
; In EnDekubaba_Init go directly into the grow state
.org 0x808fb688
; Replaces:
;   jal     EnDekubaba_SetupWait
    jal     OVL_EnDekubaba_SetupGrow

; Always go into the lunge state after growing

.org 0x808fc6a8
; Replaces:
;   jal     EnDekubaba_SetupRetract
    jal     OVL_EnDekubaba_SetupPrepareLunge


; In EnDekubaba_DecideLunge never retract
.org 0x808fcd68
; Replaces:
;   jal     EnDekubaba_SetupRetract
    jal     OVL_EnDekubaba_SetupPrepareLunge
