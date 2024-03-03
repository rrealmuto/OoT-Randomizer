; Hacks in ganondorf boss overlay
.headersize(0x809F1D00 - 0x00D7F3F0)

; Dog of Time
.org 0x809f3af4
; Replaces:
;   jal     Message_StartTextbox
;   or      a2, r0, r0
    jal     BossGanon_StartTextbox_Hack
    or      a2, s0, r0 ; Copy actor into the messagebox function.
