.n64
.relativeinclude on

// version guard, prevent people from building with older armips versions
.if (version() < 110)
.notice version()
.error   "Detected armips build is too old. Please install https://github.com/Kingcom/armips version 0.11 or later."
.endif

.create "../roms/patched.z64", 0
.incbin "../roms/base.z64"
.include "macros.asm"

;==================================================================================================
; Constants
;==================================================================================================

.include "constants.asm"
.include "addresses.asm"

;==================================================================================================
; Base game editing region
;==================================================================================================

.include "boot.asm"
.include "hacks.asm"
.include "malon.asm"
.include "mido.asm"
.include "weather.asm"

;==================================================================================================
; New code region
;==================================================================================================

.headersize (0x80400000 - 0x03480000)

.org    0x80400000
.area   0x00200000 //payload max memory
PAYLOAD_START:

.area 0x20, 0
RANDO_CONTEXT:
.word COOP_CONTEXT
.word COSMETIC_CONTEXT
.word extern_ctxt
.word AUTO_TRACKER_CONTEXT
.endarea

.include "coop_state.asm" ; This should always come first
.include "config.asm"
.include "init.asm"
.include "item_overrides.asm"
.include "cutscenes.asm"
.include "shop.asm"
.include "every_frame.asm"
.include "menu.asm"
.include "time_travel.asm"
.include "song_fix.asm"
.include "scarecrow.asm"
.include "empty_bomb.asm"
.include "initial_save.asm"
.include "textbox.asm"
.include "fishing.asm"
.include "bgs_fix.asm"
.include "chus_in_logic.asm"
.include "rainbow_bridge.asm"
.include "lacs_condition.asm"
.include "gossip_hints.asm"
.include "potion_shop.asm"
.include "jabu_elevator.asm"
.include "dampe.asm"
.include "dpad.asm"
.include "chests.asm"
.include "red_ice.asm"
.include "bunny_hood.asm"
.include "colors.asm"
.include "debug.asm"
.include "extended_objects.asm"
.include "cow.asm"
.include "lake_hylia.asm"
.include "timers.asm"
.include "shooting_gallery.asm"
.include "damage.asm"
.include "bonk.asm"
.include "bean_salesman.asm"
.include "grotto.asm"
.include "deku_mouth_condition.asm"
.include "audio.asm"
.include "king_zora.asm"
.include "carpenter_boss.asm"
.include "twinrova_wait.asm"
.include "boomerang.asm"
.include "file_select.asm"
.include "zelda.asm"
.include "link_anim.asm"
.include "malon_hooks.asm"
.include "bigocto.asm"
.include "agony.asm"
.include "horseback_archery.asm"
.include "items_as_adult.asm"
.include "carpet_salesman.asm"
.include "medigoron.asm"
.include "misc_colors.asm"
.include "door_of_time_col_fix.asm"
.include "mask_deequip.asm"
.include "trade_quests.asm"
.include "blue_fire_arrows.asm"
.include "save.asm"
.include "drop_overrides/obj_mure3.asm"
.include "drop_overrides/bg_haka_tubo.asm"
.include "drop_overrides/bg_spot18_basket.asm"
.include "drop_overrides/obj_comb.asm"
.include "drop_overrides/actor.asm"
.include "drop_overrides/enemy_drops.asm"
.include "drop_overrides/ovl_en_bb.asm"
.include "drop_overrides/ovl_en_crow.asm"
.include "drop_overrides/ovl_en_skj.asm"
.include "rand_seed.asm"
.include "messages.asm"
.include "player_save_mask.asm"
.include "gohma.asm"
.include "camera_init.asm"
.include "en_item00.asm"

.align 0x10
.importobj "../build/bundle.o"
.align 8
FONT_TEXTURE:
.incbin("../resources/font.bin")
DPAD_TEXTURE:
.incbin("../resources/dpad.bin")
TRIFORCE_ICON_TEXTURE:
.incbin("../resources/triforce_sprite.bin")

.align 0x10

.skip 0x200 ; Temporary address bump to avoid audio issues

AUDIO_THREAD_MEM_START:
.skip AUDIO_THREAD_MEM_SIZE
PAYLOAD_END:
.endarea //payload max memory

.close
