
; Resolve the address in the global pointer register to an overlay
; I guess try to do this using only temp registers
; Read the entry from the overlay table
; Calculate the RAM address as addr - VRAM Start + File RAM and jump to that address
; Overlay table at 0x800E8530
; when calling, store the address to call in t8 and the overlay index in t9
Overlay_Call_Addr:
    li  t0, 0x800E8530 ; current overlay entry
    sll t9, 5 ; multiply overlay number by 0x20 to get offset into table
    add t0, t9, t0
    lw  t1, 0x08(t0) ; current overlay VRAM start
    lw  t2, 0x0C(t0) ; current overlay VRAM end


; calculate RAM address
    lw  t3, 0x10(t0)
    sub t0, t8, t1;
    add t0, t0, t3
    jr  t0
    nop

@@end:
; Hopefully we never get here lol
jr  ra
nop