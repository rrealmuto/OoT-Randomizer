; Hacks in Obj_Mure (Spawners for fish/bugs/butterflies)
.headersize(0x80944760 - 0xCE1E70)

; Hack call to Actor_Spawn in ObjMure_SpawnActors1 so we can pass flags to the butterflies
; Loop var is in s1
; Actor pointer is in s0
; Pass them in via x/y rotation arguments. We can reset them in our hack
.org 0x80944b80
; Replaces:
;   sw      t6, 0x18(sp)
    sw      s0, 0x18(sp)

.org 0x80944b90
; Replaces:
;   sw      t7, 0x1C(sp);
    sw      s1, 0x1C(sp)

.org 0x80944b9c
; Replaces:
;   jal     Actor_Spawn
    jal     ObjMure_SpawnActors1_Actor_Spawn_Hack