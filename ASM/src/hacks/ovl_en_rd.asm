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

; Hack to not set sunSongState to  SUNSSONG_INACTIVE unless it's set to SUNSSONG_SPECIAL
; In EnRd_Stunned
.org 0x8093b5ec
; Replaces:
;   sb      r0, 0x308(s0)
;   sh      r0, -0x460e(at)
    jal     EnRd_SetSunsSongState
    sb      r0, 0x308(s0)

; And in EnRd_Update
.org 0x8093b9c8
; Replaces:
;   addiu   v0, v0, -0x5a30
;   lh      t7, 0x1422(v0)
;   beql    t7, r0, 0x8093b9ec ; Normally checks if t7 == 0 to skip setting SUNSSONG_INACTIVE
    jal     EnRd_UpdateCheckSunsSong
    addiu   v0, v0, -0x5a30
    bnel    t7, t8, 0x8093b9ec

; And in EnRd_Destroy
.org 0x80939ca0
; Replaces:
;   lh      t6, 0x1422(v0)
;   or      a0, a3, r0
;   beq     t6, r0, 0x80939cb4 ; Normally checks if t6 == 0 to skip setting SUNSSONG_INACTIVE
    jal     EnRd_DestroyCheckSunsSong
    lh      t6, 0x1422(v0)
    bne     t6, t7, 0x80939cb4