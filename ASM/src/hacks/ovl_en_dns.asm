; Hacks in en_dns (deku scrub salesman)
.headersize(0x80A74C60 - 0x00DF75A0)

; Increase the size of en_dns so we can store message ID
.org 0x80a75cec
.word 0x000002c8

; Hack EnDns_Init to store the message ID offset which we'll pass in the actor's spawn variable
; Hack towards the beginning of the function before the variable is used
.org 0x80a74c74
; Replaces:
;   lh      v0, 0x1c(s0)
;   li      at, 0x06
    jal     EnDns_Init_StoreMessage
    nop

; Hack later in EnDns_Init where it would normally store textId and use the value that we passed into the variable
; At call to Actor_SetScale, the textId is normally stored in the delay slot
.org 0x80a74d64
; Replaces:
;   jal     Actor_SetScale
;   sh      t3, 0x10e(s0)
    jal     EnDns_SetTextId
    nop

; Hack EnDns_Update to remove the whole textId nonsense. Don't think this needs to be done every frame
.org 0x80a75be0
; Replaces:
;   sh t0, 0x10e(s0)
    nop

; Hack EnDns_SetupSale to take the payment before giving the item
.org 0x80a75834
; Replaces:
;   jal     Message_CloseTextBox
;   lw      a0, 0x1c(sp)
    jal     EnDns_TakePayment
    nop

; Nop out where it normally takes the payment
.org 0x80a75958
    nop

.org 0x80a7590c
    nop
