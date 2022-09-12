;Hack for shadow temple spinning pot to drop a flagged collectible
;Flag is in a2
;Actor is in s4
;Loop index is in s0
bg_haka_tubo_hack:
lh a3, 0x18(s4)   ;get our new flag out of the z rotation
beqz a3, bh_haka_tubo_hack_end
nop
add a3, s0 ; add our loop index
;get the lower 0x3F bits and put them in the regular spot in params
andi a1, a3, 0x3F
sll a1, a1, 0x08
or a2, r0, a1 ;put the lower part of the flag in a2
;get the upper 0xC0 bits and put them in the extra space in params
andi a1, a3, 0xC0
or a2, a2, a1
bh_haka_tubo_hack_end:
jr ra
addiu a1, sp, 0x005c
