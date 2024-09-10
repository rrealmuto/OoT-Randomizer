; Hacks in en_shopnuts (Business scrub attack phase)

.headersize(0x80B40070 - 0x00EBB7A0)

; Hack at the call to Actor_Spawn so we can set an xflag in the spawning actor
.org 0x80B409E0
; Replaces
;   jal     Actor_Spawn
    jal     EnShopnuts_ActorSpawnHack

; Hack when setting up the parameters to Actor_Spawn to pass in the actor instance instead of actor context
.org 0x80B409B0
; Replaces
;   addiu   a0, a1, 0x1C24
    or      a0, s0, r0