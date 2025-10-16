.headersize (0x800110A0 - 0xA87000)
; Hack in Object_InitContext to increase the size of the object heap
; Dynamically calculate for the size of the link object
.org 0x800814c8
; Replaces:
;   jal     THA_AllocTailAlign16
    jal     Object_InitContext_AllocSpace