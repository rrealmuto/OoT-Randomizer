MOVED_ADULT_KING_ZORA:
.byte 0x00
.align 4

; Checks if King Zora should spawn as moved or not
; Returns t1 = 1 if true, else 0
kz_moved_check:
    la      t0, SAVE_CONTEXT
    lhu     t1, 0x0EDA(t0)
    andi    t1, t1, 0x0008              ; "Moved King Zora" Flag
    bnez    t1, @@return_true           ; if the flag is set, return true
    lw      t1, 0x0004(t0)              ; else, check link's age
    bnez    t1, @@return_false          ; if child, return false
    lb      t1, MOVED_ADULT_KING_ZORA   ; else, check if adult kz should be moved
    bnez    t1, @@return_true           ; if it should, return true
    nop                                 ; else, return false

@@return_false:
    jr      ra
    li      t1, 0

@@return_true:
    jr      ra
    li      t1, 1


kz_no_timer:
    ; Displaced code
    lh      t6, 0x01D0(a1)

    lw      t2, 0x01F9(a1)           ; isTrading flag
    bnez    t2, @@return             ; if not istrading, we're getting the zora tunic item
    nop
    li      t6, 0                    ; in that case, make sure we don't start the timer

@@return:
    jr      ra
    nop
