; Hacks in En_Sw (Skullwalltula/Gold Skulltula)

.headersize(0x80945770 - 0xCE2E80)

; Modify Skullwalltula behavior for enemizer
; Make them able to detect the player on the ground by removing the first 3 checks
.org 0x809478A8
ENSW_PATCH_1_ENEMIZER_START:
; Replaces:
;   lw      t6, 0x66c
    lui     t6, 0x0020
ENSW_PATCH_1_ENEMIZER_END:

; Skip the angle check
.org 0x80947944
ENSW_PATCH_2_ENEMIZER_START:
; Replaces:
;   slti    at, v1, 0x1fc2
    ori     at, r0, 0x01
ENSW_PATCH_2_ENEMIZER_END:

; Double the detection distance
.org 0x80947964
ENSW_PATCH_3_ENEMIZER_START:
; Replaces:
;   lui     at, 0x4302
    lui     at, 0x4382
ENSW_PATCH_3_ENEMIZER_END:

; Remove Bg checks during attack
; Just remove the call to the bg checking function and force v0 to 1
.org 0x809481d4
ENSW_PATCH_4_ENEMIZER_START:
; Replaces:
;   jal     0x809479e4 ; Reloc 0x2a64
    ori     v0, r0, 0x1
ENSW_PATCH_4_ENEMIZER_END:

; Patch the reloc
.org 0x80948d90
ENSW_PATCH_5_ENEMIZER_START:
    nop
ENSW_PATCH_5_ENEMIZER_END: