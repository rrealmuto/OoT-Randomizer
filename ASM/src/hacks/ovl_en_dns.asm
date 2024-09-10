; Hacks in en_dns (deku scrub salesman)
.headersize(0x80A74C60 - 0x00DF75A0)

; Increase the size of en_dns so we can store message ID (originally 0x02B8)
.org 0x80A75CEC
.word 0x000002C8

; Hook at the end of EnDns_Init, right before it returns
.org 0x80a74dc0
; Replaces:
;   lw      ra, 0x2c(sp)
;   addiu   sp, sp, 0x40
;   jr      ra
;   nop
    j       EnDns_Init_End_Hook
    nop

; Hack EnDns_Init to store the message ID offset which we'll pass in the actor's spawn variable
; Hack towards the beginning of the function before the variable is used
.org 0x80A74C74
; Replaces:
;   lh      v0, 0x1C(s0)
;   li      at, 0x06
    jal     EnDns_Init_StoreMessage
    nop

; Hack later in EnDns_Init where it would normally store textId and use the value that we passed into the variable
; At call to Actor_SetScale, the textId is normally stored in the delay slot
.org 0x80A74D68
; Replaces:
;   jal     Actor_SetScale
;   sh      t3, 0x10E(s0)
;    jal     EnDns_SetTextId
    nop

; Hack EnDns_Update to remove the whole textId nonsense. Don't think this needs to be done every frame
.org 0x80A75bE0
; Replaces:
;   sh      t0, 0x10E(s0)
    nop

; Hack EnDns_SetupSale to take the payment before giving the item
.org 0x80A75834
; Replaces:
;   jal     Message_CloseTextBox
;   lw      a0, 0x1C(sp)
    jal     EnDns_TakePayment
    nop

; Nop out where it normally takes the payment
.org 0x80A75958
    nop

.org 0x80A7590c
    nop
