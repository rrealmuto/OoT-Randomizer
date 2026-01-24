; Hacks in En_Tp (Tailpasaran)

;===============================================
; Fix Tailpasaran 0,0,0 bug
;===============================================
; So in vanilla, tailpasaran set their attack collider in EnTp_Update
; but they don't update their collider in EnTp_Update they do it in EnTp_Draw
; Not sure why it's done this way, but this causes the collider to not get updated
; If the actor isn't being drawn. This causes a scenario where if you load into
; A room but don't see the tailpasaran, it's collision will be loaded at the default
; Location of 0,0,0. It will not necessarily be culled so EnTp_Update can be called
; And this will cause it to set its collider at 0,0,0 and you can take damage
; by walking to 0,0,0

.headersize(0x808C2350 - 0xC5FBB0)

; Replace Collider_SetJntSph with our new function that moves the collider to the actor
.org 0x808C241C
; Replaces:
;   jal     Collider_SetJntSph
    jal     Collider_SetJntSph_ToActorWorld
