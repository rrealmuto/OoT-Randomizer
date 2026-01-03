; Hacks in EnGs (Gossip Stone)
.headersize(0x80B6C070 - 0xEE7790)
; ==============================================================
; Gossip Stone Shuffle
; ==============================================================

; Hack the calls to Actor_Spawn in the gossip stone action function that checks which song was played
;.org 0x80b6c2c4
;; Replaces:
;;   sw      r0, 0x18(sp) ; <-- Store actor pointer in rotX like we always do
;;   jal     Actor_Spawn
;    sw      s0, 0x18(sp)
;    jal     EnGs_SpawnFairy_Hack

; Hook gossip stone action function that is checking for a song
.org 0x80B6C1CC
; Replaces: Entire Function
    j       En_Gs_Update_Hack
    nop
