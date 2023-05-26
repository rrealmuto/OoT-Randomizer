SHUFFLE_CHEST_GAME:
.byte 0x00
.align 4

chestgame_buy_item_hook:
    lb      t0, SHUFFLE_CHEST_GAME
    beqz    t0, @@return        ; skip if the chest game isn't randomized

    la      t1, GLOBAL_CONTEXT
    lw      t0, 0x1D44(t1)      ; load scene collectible flags (Treasure Box House)
    ori     t0, t0, 0x2         ; set flag 1 (custom TCG salesman flag)
    sw      t0, 0x1D44(t1)

@@return:
    jr      ra
    sw      $zero, 0x0118(s0)   ; displaced code

chestgame_initial_message:
    lb      t0, SHUFFLE_CHEST_GAME
    beqz    t0, @@return        ; use the default message if the salesman isn't randomized

    la      t1, GLOBAL_CONTEXT
    lw      t0, 0x1D44(t1)      ; load scene collectible flags (Treasure Box Shop)
    andi    t0, t0, 0x2         ; check flag 1 (normally unused flag)
    beqz    t0, @@return        ; if the flag is not set, continue with the default message
    nop
    addiu   t8, $zero, 0x0001   ; set unk_214 = 1
    sh      t8, 0x0204(s0)      ; store value which is checked if key has already been purchased

@@return:
    jr      ra
    sw      t2, 0x0010($sp)     ; displaced code

chestgame_no_reset_flag:
    lhu     t0, 0x1402(v0)      ; removed code due to nop after jal
    lb      t1, SHUFFLE_CHEST_GAME
    bnez    t1, @@return        ; skip if chest game isn't randomized
    nop
    sw      $zero, 0x1D38(t8)   ; displaced code

@@return:
    jr      ra
    nop

chestgame_no_reset_keys:
    addiu   t2, s0, 0x0184      ; removed code due to nop after jal
    lb      t0, SHUFFLE_CHEST_GAME
    bnez    t0, @@return        ; skip if chest game isn't randomized
    nop
    sb      t9, 0x00BC(t1)      ; displaced code

@@return:
    jr      ra
    nop

chestgame_remove_chest_rng:
    mtc1    $at, $f8            ; Line replaced with jal
    lb      t1, SHUFFLE_CHEST_GAME
    beqz    t1, @@chestgame_run_chest_rng        ; skip if chest game isn't randomized and run rng
    nop
    mtc1    $zero, $f8          ; modify comparison registers
    ori     t2, $zero, 0x0001
    mtc1    t2, $f0

@@chestgame_run_chest_rng:
    jr      ra
    nop

chestgame_open_chests_separately:
    lb      t0, SHUFFLE_CHEST_GAME
    beqz    t0, @@return        ; skip if the chest game isn't randomized
    nop
    lw      ra, 0x14(sp)        ; set original return addresss
    jr ra                       ; jump out of function (skip it)
    nop

@@return:
    jr      ra
    or      a3, a1, $zero       ; displaced code

chestgame_delayed_chest_open:
    lb      t1, SHUFFLE_CHEST_GAME
    beqz    t3, @@return        ; skip if the chest game isn't randomized
    nop
    or      t9, $zero, $zero    ; set t9 to 0 so conditional always fails
    lw      a0, 0x004C($sp)     ; displaced code

@@return:
    jr      ra
    lw      a0, 0x004C($sp)     ; displaced code
