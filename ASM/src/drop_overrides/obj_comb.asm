;Hook to c function to drop our collectible for beehives
;put actor in a0
;put current params into a1.
obj_comb_hook:
or a1, a2, r0
jal obj_comb_drop_collectible
or a0, r0, a3 ; copy actor point from a3 to a0.
lw ra, 0x0014(sp)
jr ra
addiu sp, sp, 0x20