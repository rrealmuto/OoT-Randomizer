; Hacks in ovl_Fishing for fish shuffle

.headersize(0x80A34510 - 0x00DBE030)

; Increase the size of the fishing actor to store the override
.org 0x80a44a1c
.word 0x00000550 ; normally 0x0540

; Hook Actor_Spawn call in Fishing_Init
.org 0x80a35774
; Replaces
;   jal Actor_Spawn
    jal Fishing_Actor_Spawn_Hook


; Hook Fishing_Draw_Fish so we can draw overridden items
.org 0x80a40440
; Replaces
;   addiu   sp, sp, -0x30
;   sw      s0, 0x28(sp)
j      Fishing_DrawFish_Hook
nop
Fishing_DrawFish_Continue:
