CFG_DAMAGE_MULTIPLYER:
    .byte 0x00
CFG_GLOOM:
    .byte 0x00
EXTERN_DAMAGE_MULTIPLYER:
    .byte 0x00
    .align 4

Apply_Damage_Multiplier:
    li      t7, CFG_DAMAGE_MULTIPLYER
    lb      t7, 0x00(t7)
    li      t8, EXTERN_DAMAGE_MULTIPLYER
    lb      t8, 0x00(t8)
    add     t7, t7, t8

    bltz    t7, @@DivDamage
    li      at, 3
    bge     t7, at, @@ohko
    nop

@@MulDamage:
    b       @@DoubleDefence
    sllv    s0, s0, t7     ; damage multiplier

@@ohko:
    b       @@return
    sh      zero, 0x30(a1)

@@DivDamage:
    sub     t7, zero, t7
    srav    s0, s0, t7     ; damage multiplier

@@DoubleDefence:
    lbu     t7, 0x3D(a1)   ; check if has double defense
    beq     t7, zero, @@return
    nop

    sra     s0, s0, 1    ; double defense
    sll     s0, s0, 0x10
    sra     s0, s0, 0x10 ; s0 = damage

@@return:
    jr      ra
    nop

; Return point for Health_ChangeBy_Hook
Health_ChangeBy:
    addiu   sp, sp, -0x28 ; Replaced code
    j       Health_ChangeBy_Continue ; Call back into the original function
    sw      s0, 0x20(sp) ; Replaced code

; Hack when opening a save file to allow health less than 0x30
Sram_OpenSave_Health_Hack:
; Player health is in t7
; First check if health is > 0x30    
    slti    at, t7, 0x30  ; set at=1 if health < 0x30
    beqz    at, @@return ; branch if health >= 0x30
    nop
; Check gloom setting
    li      a0, CFG_GLOOM
    lb      a0, 0x00(a0)

    beqz    a0, @@not_gloom
    addiu   t8, r0, 0x30
@@gloom:
    ; Check if max health is < 0x30
    lh      a0, 0x2E(s0)
    bgt     a0, t8, @@not_gloom
    nop
@@gloom_max_low:
    ; Set the player health to the max health
    b       @@return
    sh      a0, 0x30(s0)      
@@not_gloom:
    sh      t8, 0x30(s0)
@@return:
    jr      ra
    nop

; Hack when respawning on death to set the health to min(maxHealth, 0x30)
KaleidoScope_Update_Respawn_Health_Hack:
; savectx is in t1
; t6 contains 0x30
    ; Check if max health is < 0x30
    lh      t8, 0x2E(t1)
    bgt     t8, t6, @@gt_30
    nop
@@lt_30:
    ; Set the player health to the max health
    b       @@return
    sh      t8, 0x30(t1)      
@@gt_30:
    sh      t6, 0x30(t1)
@@return:
    jr      ra
    nop
