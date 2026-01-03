; Hacks in En_Elf (Fairy/Fairy spawners/etc.) for fairy shuffle
.headersize(0x80885B00 - 0x00C233D0)

; Increase the size of the actor instance
.org 0x80889e7c
; Replaces: 0x000002C0
    .word 0x00000300

; Hack call to SkelAnime_Draw in EnElf_Draw so we can override fairy models
.org 0x80889d5c
; Replaces: jal SkelAnime_Draw
    jal     EnElf_SkelAnime_Draw_Hack


; Hack call to Actor_Spawn in EnElf_Init when the fairy spawner spawns 8 actual fairies
.org 0x80886748
; Replaces: 
;   sw      r0, 0x1c(sp) ; We're going to use this to store the actor. Actor is in s0 hopefully
;   sw      r0, 0x18(sp) ; We're going to use this to store in loop variable. Loop variable is in v1
;   jal     Actor_Spawn
    sw      s0, 0x1C(sp)
    sw      v1, 0x18(sp) ; Store loop variable in one of the params
    jal     ElElf_FairySpawn_ActorSpawn_Hack

; Hacks in the Healing fairy action function, we'll call it EnElf_HealingFairyFlyAndWait (func_80A0329C in decomp)
; We're going to make fairies give you the shuffled item when you touch them
; nop out the part the gives health/magic
.org 0x808872d0
; Replaces:
;   jal     Health_ChangeBy
;   li      a1, 0x80
;   lhu     t3, 0x2b4(so)
;   andi    t4, t3, 0x200
;   beql    t4, r0, LAB_808872f4
;   lui     at, 0x4248   ; <-- We still need this
;   jal     Magic_Fill
;   lw      a0, 0x3C(sp)
;   lui     at, 0x4248   ; <-- Again still need this
    b       0x808872f4
    lui     at, 0x4248
    nop
    nop
    nop
    nop
    nop
    nop
    nop

; Hook the call to EnElf_SetupAction 
.org 0x80887330
; Replaces:
;   jal     EnElf_SetupAction ; Reloc 0x1830
    jal     EnElf_HealingFairyFlyAndWait_SetupAction_Hook

; Hack call to Actor_OfferGetItem to prevent catching in a bottle
.org 0x808873ec
; Replaces:
;   jal     Actor_OfferGetItem
    jal     EnElf_Actor_OfferGetItem_Hack

; Patch Relocs
.org 0x8088a208 ; Reloc 0x1830
    nop