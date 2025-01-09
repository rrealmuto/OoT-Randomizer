; Hack in ovl_en_vali (Bari)

.headersize(0x8090B630 - 0xCA8DC0)

; Hack when finding the ground position to raise the bari slightly so it finds ground at ground level
.org 0x8090b768
; Replaces
;   jal     BgCheck_EntityRaycastDown4
    jal     BgCheck_EntityRaycastDown4_Raised
