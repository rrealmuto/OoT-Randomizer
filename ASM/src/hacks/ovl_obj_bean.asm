; Hacks in Obj_Bean (bean plant spot)
.headersize(0x80A80D90 - 0xE036D0)

; Hack call to Item_DropCollectible in ObjBean_GrowWaterPhase3 to spawn overridden fairies
; This is called in a loop to spawn 3 fairies. Loop variable in s0
; Actor pointer in s4
.org 0x80a82704
; Replaces:
;   or      a1, s2, r0 ; 2nd param to function which contains the spawn position. We can recalculate this
;   jal     Item_DropCollectible
;   li      a2, 0x12  ; 3rd param to function which contains the collectible to spawn. 
    or      a1, s4, r0 ; Copy actor variable into a1
    jal     Obj_Bean_Item_DropCollectible_Hack
    or      a2, s0, r0 ; Copy loop variable into a2

;; Hook ObjBean_Init to store override flag
;; Replace function in init vars
;.org 0x80a830a4
;; Replaces
;;   .word   ObjBean_Init    ; Reloc 0x2314, .data(0x14)
;    .word   ObjBean_Init_Hook
;
;; Patch relocs
;.org 0x80a83508 ; (.data)0x14
;    nop