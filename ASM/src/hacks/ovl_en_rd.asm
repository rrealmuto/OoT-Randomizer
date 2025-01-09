; Hacks in ovl_En_Rd (Redead/gibdo)
.headersize(0x80939A90 - 0xCD71B0)

; Change redead to only check xz to home
.org 0x8093a394
; Replaces:
;   jal     Actor_WorldDistXYZToPoint
    jal     Actor_WorldDistXZToPoint

.org 0x8093a6a4
; Replaces:
;   jal     Actor_WorldDistXYZToPoint
    jal     Actor_WorldDistXZToPoint

.org 0x8093a79c
; Replaces:
;   jal     Actor_WorldDistXYZToPoint
    jal     Actor_WorldDistXZToPoint
