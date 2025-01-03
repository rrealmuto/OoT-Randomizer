; Hooked call to Actor_Spawn in ObjMure2
; Need to set xflag in spawned actor
; We can probably just call Actor_Spawn and then set the data in the actor
; We need to pass in the loop index and the actor instance
; Loop index is in s1
; Actor instance is in s2 I think
; We can replace actorCtx (in a0) and the 0 that is passed into rotY via 0x1C(sp)
ObjMure2_Actor_Spawn_Hook:
    or      a0, s2, r0 ; Copy actor instance to s2
    j       ObjMure2_Actor_Spawn_Hack
    sw      s1, 0x1C(sp)
