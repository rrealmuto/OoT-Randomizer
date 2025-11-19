; Hacks in En_Skjneedle (Skull kid needles)
.headersize(0x80A703D0 - 0x00DF2D10)

; Add ACTOR_FLAG_4 (ACTOR_FLAG_UPDATE_CULLING_DISABLED) to spawn flags so that the needles don't cull
; This is because the skull kid actor itself doesn't cull and will keep
; shooting needles
; Also if needles end up out of bounds they will never despawn
.org 0x80A70644
; Replaces:
; .word 0x00000205
.word 0x00000215
