; Hacks in door_shutter (sliding doors found in dungeons)

.headersize(0x808B8370 - 0xC55C10)

; Hack when setting up the action function for DoorShutter_BarAndWaitSwitchFlag to use our custom action function
.org 0x808b84e0
; Replaces
;   lui     a1, HI(DoorShutter_BarAndWaitSwitchFlag)
;   addiu   a1, a1, LOW(DoorShutter_barAndWaitSwitchFlag)
    li      a1, DoorShutter_BarAndWaitSwitchFlag_Override ; Reloc 0x170 and 0x174

; Relocs
.org 0x808ba3f8 ; 0x170 + 0x174
nop
nop
