; Hacks in Shot_Sun (Shoot the sun target and sun's song/song of storms spot??) Why are these the same actor?
.headersize(0x80B226C0 - 0xE9DDF0)

; Hack call to Actor_Spawn in ShotSun_SpawnFairy so we can override the fairy
.org 0x80b22838
; Replaces:
;   sw      r0, 0x18(sp) ; <- pass actor pointer in rotX
    sw      s0, 0x18(sp)

.org 0x80b22844
; Replaces:
;   jal     Actor_Spawn
    jal     ShotSun_SpawnFairy_ActorSpawn_Hack