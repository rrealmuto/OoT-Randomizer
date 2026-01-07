.headersize (0x800110A0 - 0xA87000)
;==================================================================================================
; Damage Multiplier
;==================================================================================================

; Replaces:
;   lbu     t7, 0x3d(a1)
;   beql    t7, zero, 0x20
;   lh      t8, 0x30(a1)
;   bgezl   s0, 0x20
;   lh      t8, 0x30(a1)
;   sra     s0, s0, 1    ; double defense
;   sll     s0, s0, 0x10
;   sra     s0, s0, 0x10 ; s0 = damage

; Hack in Health_ChangeBy to adjust the amount of damage taken based on the setting
; Hack is currently where the code checks for double defense
.org 0x8007211C
    bgez    s0, @@continue ; check if damage is negative
    lh      t8, 0x30(a1)   ; load hp for later
    jal     Apply_Damage_Multiplier
    nop
    lh      t8, 0x30(a1)   ; load hp for later
    nop
    nop
    nop
@@continue:

;==================================================================================================
; Gloom
;==================================================================================================
; Hook the entire Health_ChangeBy function
.org 0x800720bc
; Replaces:
;   addiu   sp, sp, -0x28
;   sw      s0, 0x20(sp)
    j       Health_ChangeBy_Hooked
    nop
Health_ChangeBy_Continue:

; Hack when opening a save file to allow health less than 0x30
; In Sram_OpenSave when it checks the health
.org 0x800903f8
; Replaces:
;   lh      t7, 0x30(s0)
;   li      t8, 0x30
;   lui     a0, 0x8010
;   slti    at, t7, 0x30  ; set at=1 if health < 0x30
;   beq     at, r0, 0x80090414 ; branch if health >= 0x30
;   lui     a1, 0x8012
;   sh      t8, 0x30(s0)
    jal     Sram_OpenSave_Health_Hack
    lh      t7, 0x30(s0)
    lui     a0, 0x8010
    lui     a1, 0x8012
    nop
    nop
    nop


.headersize(0x808137c0 - 0xbb11e0) ; Kaleidoscope
; Hack when respawning on death to set the health to min(maxHealth, 0x30)
; In KaleidoScope_Update when setting health to 0x30
.org 0x80828870
; Replaces:
;   sb      a2, 0x141d(t1) ; Unrelated to this hack
;   sh      t6, 0x30(t1) ; t6 normally contains 0x30 and is being put into the health variable
    jal     KaleidoScope_Update_Respawn_Health_Hack
    sb      a2, 0x141d(t1) ; Replaced code