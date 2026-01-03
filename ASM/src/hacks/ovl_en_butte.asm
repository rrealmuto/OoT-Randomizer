; Hacks in En_Butte (Butterflies)
.headersize(0x80A598A0 - 0x00DDC2E0)

; Hack call to Actor_Spawn in EnButte_TransformIntoFairy to assign xflag
.org 0x80a5a86c
; Replaces:
;   sw      r0, 0x18(sp) ; <-- Going to use this to store EnButte pointer. This should be rotX param
    sw      s0, 0x18(sp)
.org 0x80a5a888
; Replaces:
;   jal     Actor_Spawn
    jal     EnButte_SpawnFairy_Hack

; Hack call to EnButte_SetupFollowLink so we can make them easier to trigger if they're shuffled
.org 0x80a5a354
; Replaces:
;   jal     EnButte_SetupFollowLink
    jal     EnButte_SetupFollowLink_Hack ; <- Reloc 0x0ab4

; Relocs
.org 0x80a5ada8 ; 0x0ab4
    nop