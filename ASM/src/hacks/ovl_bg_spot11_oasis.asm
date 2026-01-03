; Hacks in Bg_Spot11_Oasis - Thing in desert collosus that spawns fairies when you play song of storms
.headersize(0x80B757E0 - 0xEF0F00)

; Hack call to Actor_Spawn in the function that does so, so we can set a flag for the Fairy spawner that it spawns
.org 0x80b75a60
; Replaces:
;   sw      r0, 0x18(sp) ; <-- We'll store the actor pointer here, should be rotX
;   jal     Actor_Spawn
    sw      s0, 0x18(sp)
    jal     BgSpot11Oasis_SpawnFairies_ActorSpawn_Hack