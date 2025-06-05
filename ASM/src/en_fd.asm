; Hack in EnFd_Update to not play the miniboss BGM in enemizer
; Need to put 1/0 in t6 to determine whether or not to play BGM (0 should skip playing BGM)
; s0 contains actor instance
; original code is   lh      t6, 0x4ac(s0) which checks if this is the first time that the update function was run
; also need to store 0 to 0x4ac(s0) if we're going to skip setting the BGM
EnFd_BGM_Hack:
; Check if enemizer is enabled
    lb      t6, CFG_ENEMIZER
    bgtz    t6, @@return ; branch if enemizer is enabled
    nop
    lh      t6, 0x4ac(s0) ; Replaced code - checks if this is the first time that the update function was called

@@return:
    jr      ra
