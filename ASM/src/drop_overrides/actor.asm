CURR_ACTOR_SPAWN_INDEX:
.halfword 0x0000
.halfword 0x0000

Actor_SetWorldToHome_Hook:	
    addiu   sp, sp, -0x20	
    sw      ra, 0x1C (sp)	
    jal     Actor_SetWorldToHome_End	
    nop	
    lw      ra, 0x1C (sp)	
    jr      ra	
    addiu   sp, sp, 0x20

; Hacks Actor_UpdateAll so that we can override actor spawns.
Actor_UpdateAll_Hook:
;A0 - Actor Context
;A1 - Actor Entry
;A2 - Global Context
;S0 - Actor Index (0 indexed. Add 1 when storing to the actor)
;V0 - after the call to Actor_Spawn will contain the address of the actor
    addiu   sp, sp, -0x50
    sw      t0, 0x20(sp)
    sw      t1, 0x24(sp)
    sw      t2, 0x28(sp)
    sw      a0, 0x2C(sp)
    sw      a1, 0x30(sp)
    sw      a2, 0x34(sp)
    sw      s0, 0x38(sp)
    sw      s1, 0x3C(sp)
    sw      ra, 0x40(sp)

    addiu   s0, 1
    sh      s0, CURR_ACTOR_SPAWN_INDEX ; Store the actor index in a global variable so we can easily pass it into other functions

    jal     0x800255C4 ; Call Actor_SpawnEntry
    nop

    lw      a1, 0x30(sp)

    ; Make sure the actor was spawned
    beqz    v0, @@end
    nop
@@call_hack:
    or      a0, r0, v0 ;move the actor pointer to a0
    lw      a1, 0x34(sp) ; Get the game context off the stack to pass into the hack.
    jal     Actor_After_UpdateAll_Hack ; Call our hack
    nop
@@end:
    lw      t0, 0x20(sp)
    lw      t1, 0x24(sp)
    lw      t2, 0x28(sp)
    lw      a0, 0x2C(sp)
    lw      a1, 0x30(sp)
    lw      a2, 0x34(sp)
    lw      s0, 0x38(sp)
    lw      s1, 0x3C(sp)
    lw      ra, 0x40(sp)
    jr      ra
    addiu   sp, sp, 0x50 

; Hacks the call to ZeldaArena_Malloc when spawning an actor in Actor_Spawn to increases the size of the actor
; v1 - ActorInit struct from actor overlay table
; a0 - size of the actor
Actor_Spawn_Malloc_Hack:
    addiu   sp, sp, -0x20
    sw      ra, 0x10(sp)
    sw      s0, 0x14(sp)
; Check if this is the player actor. Don't want to increase this.
    lh      s0, 0x00(v1) ; Get the ID from the entry
    beqz    s0, @spawn
    nop
    addiu   a0, a0, 0x10 ; Increase the size of the actor 
@spawn:
    jal     0x80066C10 ; (ZeldaArena_Malloc)
    nop  
    lw      s0, 0x14(sp)
    lw      ra, 0x10(sp)
    jr      ra
    addiu   sp, sp, 0x20

Actor_Spawn_Shift:
    addiu   sp, sp, -0x20
    sw      s0, 0x10(sp)

    ; Get the ActorInit from the stack. it was at 0x4C(sp) but we just moved down 0x20
    lw      s0, 0x6C(sp)
    lh      s0, 0x00(s0)
    beqz    s0, @end
    nop
    addiu   v0, v0, 0x10
    ; This is code that occurs in a delay slot in a branch earlier in the code. Need to call it again because it used v0
    or      a3, v0, r0
@end:
    lw      s0, 0x10(sp)
    ; Replaced code
    lb      t2, 0x001E(s0)
    lui     at, 0x0001
    jr      ra
    addiu   sp, sp, 0x20
