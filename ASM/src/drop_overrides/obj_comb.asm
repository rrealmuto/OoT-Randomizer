;Hack beehives to drop a collectible w/ an extended flag, based on the grotto param?
;actor pointer should be in a3. also at 0x20(sp)
;globalContext pointer in 0x0024(sp)
obj_comb_hack:
lh a2, 0x001e(sp)
lui a3, 0x8011
ori a3, a3, 0xA5D0 ;put z64_file address in a3
lb a3, 0x1397(a3) ;get the grotto id
andi a3, a3, 0x1F
addiu a3, a3, 0x60 ;add 0x60 so we dont conflict with our flags for the cow grotto rupee tower.
;build our flag
;get the lower 0x3F bits and put them in the regular spot in params
andi a1, a3, 0x3F
sll a1, a1, 0x08
or a2, a2, a1 ;put the lower part of the flag in a2
;get the upper 0xC0 bits and put them in the extra space in params
andi a1, a3, 0xC0
or a2, a2, a1

jr ra
lw a3, 0x20(sp)
