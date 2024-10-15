; Hacks for ovl_obj_mure2 (Grass/Rock Circles) to add flags when spawning actors
.headersize(0x80ABD750 - 0xE3D5A0)

; Hack call to Actor_Spawn
.org 0x80abdbcc
; Replaces
;   jal     Actor_Spawn
    jal     ObjMure2_Actor_Spawn_Hook
