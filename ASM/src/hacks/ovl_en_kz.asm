; King Zora hacks
.headersize(0x80AD5D60 - 0x00E55BA0)

; Hack call to actor_spawnaschild so we can replace the red ice block
.org 0x80ad6810
; Replaces
;   jal     Actor_SpawnAsChild
    jal     EnKz_SpawnRedIce_Hook

; Override EnKz_Update in ActorInit
.org 0x80ad7028; (0x12C8 (0x18 into data))
.word EnKz_Update_Hook

; Patch the reloc
; 0x12c8 (data 0x18)
.org 0x80ad7230
    nop

;==================================================================================================
; King Zora Init Moved Check Override
;==================================================================================================
; Replaces: lhu     t0, 0x0EDA(v0)
;           or      a0, s0, zero
;           andi    t1, t0, 0x0008

.org 0x80ad6790
    jal     kz_moved_check
    nop
    or      a0, s0, zero