; Hacks in en_g_switch (Silver rupees and some other things)
.headersize(0x80A706E0 - 0xDF3020)

; Remove the GTG Boulder room behavior
; In EnGSwitch_SilverRupeeTracker when it checks the current room
.org 0x80a70c5c
; Replaces
;   lh      t8, 0xa4(a3)
;   li      at, 0x0b
    b       0x80a70c90 ; Just always jump into the default behavior
    nop