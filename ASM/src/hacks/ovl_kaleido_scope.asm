; Hacks in ovl_kaleido_scope

.headersize(0x808137c0- 0xBB11E0)

;=====================================================================================================
; Override item equip requirements checks
;=====================================================================================================

; There are a few of these and they're inlined macros that check the age requirements using either gSlotAgeReqs or gItemAgeReqs
; gSlotAgeReqs - table with indices corresponding to inventory slot numbers.
; #define CHECK_AGE_REQ_SLOT(slot) ((gSlotAgeReqs[slot] == AGE_REQ_NONE) || gSlotAgeReqs[slot] == ((void)0, gSaveContext.save.linkAge))
; gItemAgeReqs - table with indices correspond to item IDs
; #define CHECK_AGE_REQ_ITEM(item) ((gItemAgeReqs[item] == AGE_REQ_NONE) || (gItemAgeReqs[item] == ((void)0, gSaveContext.save.linkAge)))

;8081a72c - Sets the size of the item when you cursor over it if you are the right age
;8081a2f4 - does the equip when you press the c button
;8081a270 -  Set the color used to draw the item name
; another in DrawAmmoCount
;80826608 - in KaleidoScope_Update this is what grays out the texture. This calls CHECK_AGE_REQ_ITEM which is different than CHECK_AGE_REQ_SLOT

; Hack the function that sets the size
.org 0x8081a71c
; Replaces
;   *lui     v0, 0x8083
;   *addu    v0, v0, t1
;   bnel    t6, r0, 0x8081a92c
;   lw      t0, 0x2c0(s2)
;   *lbu     v0, -0x628C(v0)
;   *li      at, 0x9
;   lui     t8, 0x8083
;   *beq     v0, at, 0x8081a74c  ; Reloc 0x6f78
;   *nop
;   *lw      t7, 0x04(s3)
;   *bnel    v0, t7, 0x8081a92c
;   lw      t0, 0x2c0(s2)

    nop
    nop
    .skip(8)
    nop
    or      v0, t1, r0
    .skip(4)
    jal     item_menu_check_age_requirements_hook
    nop
    nop
    beqz    v0, 0x8081a92c


; Prevent empty slots from being equipped
; Replaces:
;   lbu     v0, 0x0000 (s1)
;   addiu   at, r0, 0x0009
;   beq     v0, at, 0x8081a30c
;   li      at, 0x2C
;   lw      t8, 0x4(s3)
;   bne     v0, t8, 0x8081a480
.org 0x8081a2f0 ; In memory: 0x8038F690
    jal     item_menu_prevent_empty_equip
    nop
    addiu   at, r0, 0xFF ; Check if our hack returned 0xFF for empty slot
    beq     v0, at, 0x8081a480
    nop
    beqz    v0, 0x8081a480 ; Check if our hack returned 0 to prevent equip

; Hack the function that lets you equip
;.org 0x8081a2f4
; Replaces
;   li      at, 0x09
;   beql    v0, at, 0x8081a30c ; Reloc 0x6B38
;   li      at, 0x2c
;   lw      t8, 0x4(s3)
;   bne     v0, t8, 0x8081a480

;    jal     item_menu_check_age_requirements_hook
;    nop
;    nop
;    nop
;    beqz    v0, 0x8081a480

; Hack the function that draws the item name?
.org 0x8081a270
; Replaces
;   beq     v0, at, 0x8081a290
;   li      at, 0x3e7
;   lw      t8, 0x04(s3)
;   li      t9, 0x01
;   beq     v0, t8, 0x8081a290

    jal     item_menu_check_age_requirements_hook
    or      v0, t6, r0
    nop
    li      t9, 0x01
    bnez    v0, 0x8081a290

; Hack the function that grays out the texture
; This is called in a loop over all item IDs
; Pointers to the 32x32 RGBA textures are stored in the table gItemIcons
; Indices in the table correspond to the item ID
.org 0x80826608
; Replaces
;   *lui     v0, 0x8083;    reloc 0x12e48
;   *addu    v0, v0, v1
;   *lbu     v0, -0x6264(v0); reloc 0x12e50
;   *li      at, 0x09
;   *beq     v0, at, 0x80826684
;   *addiu   v1, v1, 0x01
;   *lw      t7, 0x04(t1)
;   *lui     at, 0xff
;   *ori     at, at, 0xffff
;   *beq     v0, t7, 0x80826680

    nop
    nop
    lui     at, 0xff
    ori     at, at, 0xffff
    nop
    nop
    nop
    jal     item_menu_check_age_requirements_item_hook
    or      v0, v1, r0
    bnez    v0, 0x80826680

; Relocs
.org 0x8082f138
    nop
.org 0x8082f13c
    nop
.org 0x8082fb48 ; 0x12e48
    nop
.org 0x8082fb4c ; 0x12E50
    nop