; Hacks in ovl_Fishing for fish shuffle

.headersize(0x80A34510 - 0x00DBE030)

; Increase the size of the fishing actor to store the override
.org 0x80a44a1c
.word 0x00000570 ; normally 0x0540

; Hook Actor_Spawn call in Fishing_Init
.org 0x80a35774
; Replaces
;   jal Actor_Spawn
    jal Fishing_Actor_Spawn_Hook


; Hook Fishing_DrawFish call to SkelAnime_DrawFlexOpa to draw our model
.org 0x80a405d8
; Replaces
;   jal SkelAnime_DrawFlexOpa
    jal Fishing_SkeletonDraw_Hook

.org 0x80a4066c
; Replaces
;   jal SkelAnime_DrawFlexOpa
    jal Fishing_SkeletonDraw_Hook