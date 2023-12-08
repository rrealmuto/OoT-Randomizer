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

; Hook Fishing_UpdateFish when it calls Actor_Kill so we can give the overridden item
.org 0x80a3f3e0
; Replaces
;   jal Actor_Kill
    jal Fishing_GiveOverride_Kill

; Hook when a fish is caught (case D 6) in Fishing_UpdateFish to keep track of the fish that was caught
.org 0x80a3ef3c
; Replaces
;   jal 0x80064624
    jal Fish_Caught_Hook

; Hook the call to Message_StartTextbox in Fishing_UpdateOwner so we can override the message
.org 0x80a42b98
; Replaces
;   jal Message_StartTextbox
    jal Fishing_CaughtFish_Textbox

; Hook call to Fishing_HandleOwnerDialog so we can handle our own dialog for fish shuffle
.org 0x80a42ba8
; Replaces
;   jal Fishing_HandleOwnerDialog - This is relocated so clear the relocation
    jal Fishing_HandleOwnerDialog_Hook


; Relocs
.org 0x80a47734 ; Call to Fishing_HandleOwnerDialog
nop