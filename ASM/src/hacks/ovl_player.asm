; Hacks in player overlay

.headersize(0x808301C0 - 0x00BCDB70)

.org 0x80834254
NNN_PATCH_1_START:
nop
nop
nop
nop
NNN_PATCH_1_END:

.org 0x80834268
NNN_PATCH_2_START:
ori     a0, r0, 0x6858
NNN_PATCH_2_END:

; Reloc (0x4094)

.org 0x80853858
NNN_PATCH_3_START:
nop
NNN_PATCH_3_END:

; Gold boulder hack
; In Player_ActionHandler_2 where it checks the boulder type from the actor variable and checks the player's strength
.org 0x8083caac
; Replaces:
;   lh      t7, 0x1c(t5) ; Puts the actor variable into t7.
;   li      at, 0x01
;   andi    t8, t7, 0x0F ; and actor variable with 0x0F (takes lowest 4 bits). This is perfect since bits 1-3 are normally unused
;   bne     t8, at, 0x8083cad0 ; if actor variable != 1, continue in the function
;   slti    at, v0, 0x2 ; sets at if v0 < 0x2 (v0 contains player's strength level)
;   beqz     at, 0x8083cad0 ; if at = 0 continue to lift the boulder
;   nop
;   b       0x8083caf8      ; return 0
;   or      v0, r0, r0
; So for the hack to work we want to set at to 0 if we can lift the boulder
    lh      t7, 0x1c(t5)
    li      at, 0x01
    andi    t8, t7, 0x0F
    jal     Player_CanLiftIshi
    lh      t7, 0x1c(t5)
.headersize(0x808301C0 - 0xBCDB70)

; Hack in Player_PlayVoiceSfx so we can adjust volume
; Hack the call to Player_PlaySfx to call our own version
.org 0x808306A8
; Replaces:
;   jal     Player_PlaySfx
    jal     Player_PlaySfxWithVolume
