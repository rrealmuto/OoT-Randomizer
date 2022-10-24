;Hack obj_tsubo_spawn function to set drop_collectible_override_flag
obj_tsubo_spawn_hook:
    lh      a0, 0x18(a0)
    li      a2, drop_collectible_override_flag
    sh      a0, 0x00(a2)

; Replaced code
    lw      a0, 0x001c(sp)
    sra     t6, v1, 9
    jr      ra
    andi    t7,t6, 0x003F