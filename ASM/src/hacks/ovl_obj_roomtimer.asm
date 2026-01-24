; Hacks in Obj_Roomtimer
.headersize(0x80B25340 - 0xEA0A70)

; In the main action function, replace the call to Flags_GetTempClear with a custom function so we can override the default behavior
.org 0x80b2543c
; Replaces:
;   lw      a0, 0x2c(sp)
;   jal     Flags_GetTempClear
;   lb      a1, 0x3(s0)
    jal     ObjRoomTimer_ConditionCheckHack
    nop
    nop
