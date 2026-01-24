; Hacks in en_mb (Moblin)
.headersize(0x808E5D40 - 0xC83500)

; Increase the size of actor
.org 0x808e981c
; Replaces:
;   .dw     0x4FC
    .dw     0x50C

; Hook EnMb_Init
; Update pointer in Actor Init
.org 0x808e9820
.dw    EnMb_Init_Hooked

; Replace all calls to EnMb_SetupClubWaitPlayerNear to use our new function
.org 0x808e6de8; reloc 0x10A8
    jal     EnMb_SetupClubWaitPlayerNear_Hooked
.org 0x808e71a0; reloc 0x1460
    jal     EnMb_SetupClubWaitPlayerNear_Hooked
.org 0x808e806c; reloc 0x232C
    jal     EnMb_SetupClubWaitPlayerNear_Hooked
.org 0x808e8178; reloc 0x2438
    jal     EnMb_SetupClubWaitPlayerNear_Hooked

; Relocs
.org 0x808e9e10 ; (EnMb_Init in actor var)
    nop
; Calls to EnMb_SetupCluBWaitPlayerNear
.org 0x808e9c74
    nop
.org 0x808e9c9c
    nop
.org 0x808e9cfc
    nop
.org 0x808e9d00
    nop

; Hack in EnMb_Init to add additional moblin type
; Hack at the else case where it normally handles the patrolling kind
;.org 0x808e5ffc
; Replaces:
;   lui     a2, 0x601
;   lui     a3, 0x600
;    jal     EnMb_Init_CheckExtraCases
;    nop
