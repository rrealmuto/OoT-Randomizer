Actor_Spawn_Continue:
    addiu   sp, sp, -0x58
    j       Actor_Spawn_Continue_Jump_Point
    sw      a2, 0x0060(sp)
