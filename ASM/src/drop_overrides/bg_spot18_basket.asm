;goron spinning pot hacks to drop override collectibles

;hack for when it drops bombs
;Loop variable stored in s7
;flag needs to be stored in a2
;Actor pointer is stored in s0 hopefully
bg_spot18_basket_bombs_hack:
lh a3, 0x18(s0)   ;get our new flag out of the z rotation
ori a2, r0, 0x0000 ; clear a2
beqz a3, bg_spot18_basket_bombs_end
nop
add a3, s7 ; add our loop index
;get the lower 0x3F bits and put them in the regular spot in params
andi a1, a3, 0x3F
sll a1, a1, 0x08
or a2, r0, a1 ;put the lower part of the flag in a2
;get the upper 0xC0 bits and put them in the extra space in params
andi a1, a3, 0xC0
or a2, a2, a1
bg_spot18_basket_bombs_end:
jr ra
or a1, s3, r0
