; Hacks in obj_hana for grass textures match contents
.headersize(0x80ABC010 - 0xE3BE60)

; Replace Draw function in ActorInit
.org 0x80abc22c
.word   ObjHana_Draw_Hack

; Remove it from the relocation list
.org 0x80abc310
nop
