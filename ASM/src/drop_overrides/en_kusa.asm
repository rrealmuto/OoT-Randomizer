; Hook called at the start of EnKusa_DropCollectible
; Call our drop override function
; Check the return address of the drop override function and decide whether to continue
; Into the original function, or return
; We pushed the stack 0x18 before this call
; And we put the original RA at 0x10(sp)
enkusa_dropcollectible_hook:
    addiu   sp, sp, -0x20
    sw      ra, 0x14(sp) ; store the RA inside EnKusa_DropCollectible
    sw      a0, 0x18(sp) ; store a0/a1
    sw      a1, 0x1c(sp)
    ; Call our hack
    jal     enkusa_dropcollectible_hack
    nop
    ; check if we should continue on in the function or just return
    ; If the hack returns 0, continue in the original function
    ; If the hack returns 1, return to the caller

    beqz    v0, @@continue
    nop
@@return_to_caller:
    ; Get the original RA
    addiu   sp, sp, 0x20
    lw      ra, 0x10(sp)
    jr      ra
    addiu   sp, sp, 0x18
@@continue:
    ; Continue into the original function
    ; Set up a0/a1/ra
    lw      a0, 0x18(sp)
    lw      a1, 0x1c(sp)
    lw      ra, 0x14(sp) ; This is the address we want to return to
    addiu   sp, sp, 0x20
    lw      v0, 0x10(sp) ; Put the original RA somewhere so we can use it
    ; Replaced code so everything is where it wants to be
    sw      v0, 0x14(sp) ; Put the original RA where it is supposed to be
    sw      a0, 0x18(sp)
    jr      ra           ; Return into the original function
    sw      a1, 0x1c(sp)
