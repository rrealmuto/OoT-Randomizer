from enum import Enum
from math import ceil
import sys
from Audiobank import *
from Rom import Rom
from Settings import Settings
from Utils import data_path
import os
import aifc
import numpy as np

AUDIOBANK_INDEX_ADDR = 0x00B896A0
AUDIOBANK_FILE_ADDR = 0xD390
AUDIOBANK_FILE_LENGTH = 0x1CA50
AUDIOTABLE_FILE_ADDR = 0x79470
AUDIOTABLE_FILE_LENGTH = 0x460AD0
AUDIOTABLE_INDEX_ADDR = 0x00B8A1C0

child_link_sfx = [
    ("Child Link - Dying Gasp", 20),
    ("Child Link - Attack 1", 28),
    ("Child Link - Attack 2", 29),
    ("Child Link - Attack 3", 30),
    ("Child Link - Attack 4", 31),
    ("Child Link - Strong Attack 1", 32),
    ("Child Link - Strong Attack 2", 33),
    ("Child Link - Dangling Gasp 1", 34),
    ("Child Link - Sigh 1", 35),
    ("Child Link - Sigh 2", 36),
    ("Child Link - Hurt 1", 37),
    ("Child Link - Hurt 2", 38),
    ("Child Link - Hurt 3", 39),
    ("Child Link - Hurt 4", 40),
    ("Child Link - Knocked Back", 41),
    ("Child Link - Hurt 5", 42),
    ("Child Link - Falling 1", 43),
    ("Child Link - Falling 2", 44),
    ("Child Link - Gasp 1", 45),
    ("Child Link - Gasp 2", 46),
    ("Child Link - Wheeze", 47),
    ("Child Link - Exhausted Panting", 48),
    ("Child Link - Painful Landing", 49),
    ("Child Link - Danging Gasp 2", 50),
    ("Child Link - Glug", 51),
    ("Child Link - Refreshed", 52),
    ("Child Link - Hup 1", 53),
    ("Child Link - Hup 2", 54),
    ("Child Link - Charge Up", 62),
    ("Child Link - Cast Spell", 63),
    ("Child Link - Dying Gasp", 64),
    ("Child Link - Strangled", 65),
    ("Child Link - Gasp 3", 66),
    ("Child Link - Shiver", 67),
    ("Child Link - Sneeze", 69),
    ("Child Link - Grunt", 70),
    ("Child Link - Moans From Heat", 71),
    ("Child Link - Awakening Grunt", 72),
    ("Child Link - Wiping Off Sweat", 73),
    ("Child Link - Yawn Starts", 74),
    ("Child Link - Yawn and Stretch", 75),
    ("Child Link - Stretch Refreshingly", 76),
    ("Child Link - Dramatic Gasp", 135)
]

adult_link_sfx = [
    ("Adult Link - Attack 1", 0x00),
    ("Adult Link - Attack 2", 0x01),
    ("Adult Link - Attack 3", 0x02),
    ("Adult Link - Attack 4", 0x03),
    ("Adult Link - Strong Attack 1", 0x04),
    ("Adult Link - Strong Attack 2", 0x05),
    ("Adult Link - Dangling Grunt", 0x06),
    ("Adult Link - Climb Edge", 0x07),
    ("Adult Link - Dangling Gasp 1", 0x08),
    ("Adult Link - Hurt 1", 0x09),
    ("Adult Link - Hurt 2", 0x0A),
    ("Adult Link - Hurt 3", 0x0B),
    ("Adult Link - Hurt 4", 0x0C),
    ("Adult Link - Knocked Back", 0x0D),
    ("Adult Link - Hurt 5", 0x0E),
    ("Adult Link - Falling 1", 0x0F),
    ("Adult Link - Falling 2", 0x10),
    ("Adult Link - Gasp 1", 0x11),
    ("Adult Link - Gasp 2", 0x12),
    ("Adult Link - Pant 1", 0x13),
    ("Adult Link - Spur Horse 1", 0x15),
    ("Adult Link - Spur Horse 2", 0x16),
    ("Adult Link - Pant 2", 0x17),
    ("Adult Link - Painful Landing", 0x18),
    ("Adult Link - Dangling Gasp 2", 0x19),
    ("Adult Link - Hup", 0x1A),
    ("Adult Link - Gasp 3", 0x1B),
    ("Adult Link - Glug", 0x37),
    ("Adult Link - Refreshed", 0x38),
    ("Adult Link - Lift", 0x3C),
    ("Adult Link - Cast Spell", 0x3D),
    ("Adult Link - Hurt 6", 0x4D),
    ("Adult Link - Choking", 0x4E),
    ("Adult Link - Gasping", 0x4F),
    ("Adult Link - Small Gasp", 0x50),
    ("Adult Link - Unsettled Moan", 0x51),
    ("Adult Link - Sneeze", 0x52),
    ("Adult Link - Sigh 1", 0x53),
    ("Adult Link - Sigh 2", 0x54),
    ("Adult Link - Sigh 3", 0x55),
    ("Adult Link - Stretch Start", 0x56),
    ("Adult Link - Stretching", 0x57),
    ("Adult Link - Finished Stretching", 0x58),
    ("Adult Link - Dramatic Gasp", 0x86)
]

voice_link_adult_attack
voice_link_adult_attack_long
voice_link_adult_epona
voice_link_adult_ledge_grab
voice_link_adult_ledge_climb
voice_link_adult_hurt
voice_link_adult_frozen
voice_link_adult_fall_short
voice_link_adult_fall_long
voice_link_adult_low_health
voice_link_adult_drink_gasp
voice_link_adult_death
voice_link_adult_wallmaster
voice_link_adult_grabbed
voice_link_adult_sneeze
voice_link_adult_sweat
voice_link_adult_drink
voice_link_adult_idle
voice_link_adult_sword_swing_unused
voice_link_child_shiver
voice_link_adult_jump
voice_link_adult_cast_spell1
voice_link_adult_shock
voice_link_adult_cast_spell2
voice_link_adult_push_block
voice_link_adult_hookshot_hang
voice_link_adult_knockback
voice_link_adult_unused
voice_link_adult_cast_spell3
voice_link_adult_fall_long
voice_link_adult_knockback_unused
voice_link_adult_attack
voice_link_child_attack
voice_link_child_attack_long
voice_link_adult_epona
voice_link_child_ledge_grab
voice_link_child_ledge_climb
voice_link_child_hurt
voice_link_child_frozen
voice_link_child_fall_short
voice_link_child_fall_long
voice_link_child_low_health
voice_link_child_drink_gasp
voice_link_child_death
voice_link_child_wallmaster
voice_link_child_grabbed
voice_link_child_sneeze
voice_link_child_sweat
voice_link_child_drink
voice_link_child_idle
voice_link_child_sword_swing_unused
voice_link_child_shiver
voice_link_child_jump
voice_link_child_cast_spell1
voice_link_child_shock
voice_link_child_cast_spell2
voice_link_child_push_block
voice_link_child_hookshot_hang
voice_link_child_knockback
voice_link_child_unused
voice_link_child_cast_spell3
voice_link_child_fall_long
voice_link_child_cutscene_attacked
voice_link_adult_attack

/* 0x6800 */ DEFINE_SFX(NA_SE_VO_LI_SWORD_N, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6801 */ DEFINE_SFX(NA_SE_VO_LI_SWORD_L, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6802 */ DEFINE_SFX(NA_SE_VO_LI_LASH, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6803 */ DEFINE_SFX(NA_SE_VO_LI_HANG, 0x20, 2, 0, SFX_FLAG_10)
/* 0x6804 */ DEFINE_SFX(NA_SE_VO_LI_CLIMB_END, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6805 */ DEFINE_SFX(NA_SE_VO_LI_DAMAGE_S, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6806 */ DEFINE_SFX(NA_SE_VO_LI_FREEZE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6807 */ DEFINE_SFX(NA_SE_VO_LI_FALL_S, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6808 */ DEFINE_SFX(NA_SE_VO_LI_FALL_L, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6809 */ DEFINE_SFX(NA_SE_VO_LI_BREATH_REST, 0x30, 2, 1, SFX_FLAG_10)
/* 0x680A */ DEFINE_SFX(NA_SE_VO_LI_BREATH_DRINK, 0x30, 2, 1, SFX_FLAG_10)
/* 0x680B */ DEFINE_SFX(NA_SE_VO_LI_DOWN, 0x30, 2, 0, SFX_FLAG_10)
/* 0x680C */ DEFINE_SFX(NA_SE_VO_LI_TAKEN_AWAY, 0x30, 2, 0, SFX_FLAG_10)
/* 0x680D */ DEFINE_SFX(NA_SE_VO_LI_HELD, 0x50, 2, 0, SFX_FLAG_10)
/* 0x680E */ DEFINE_SFX(NA_SE_VO_LI_SNEEZE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x680F */ DEFINE_SFX(NA_SE_VO_LI_SWEAT, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6810 */ DEFINE_SFX(NA_SE_VO_LI_DRINK, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6811 */ DEFINE_SFX(NA_SE_VO_LI_RELAX, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6812 */ DEFINE_SFX(NA_SE_VO_LI_SWORD_PUTAWAY, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6813 */ DEFINE_SFX(NA_SE_VO_LI_GROAN, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6814 */ DEFINE_SFX(NA_SE_VO_LI_AUTO_JUMP, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6815 */ DEFINE_SFX(NA_SE_VO_LI_MAGIC_NALE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6816 */ DEFINE_SFX(NA_SE_VO_LI_SURPRISE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6817 */ DEFINE_SFX(NA_SE_VO_LI_MAGIC_FROL, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6818 */ DEFINE_SFX(NA_SE_VO_LI_PUSH, 0x30, 2, 2, SFX_FLAG_10)
/* 0x6819 */ DEFINE_SFX(NA_SE_VO_LI_HOOKSHOT_HANG, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681A */ DEFINE_SFX(NA_SE_VO_LI_LAND_DAMAGE_S, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681B */ DEFINE_SFX(NA_SE_VO_LI_NULL_0x1b, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681C */ DEFINE_SFX(NA_SE_VO_LI_MAGIC_ATTACK, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681D */ DEFINE_SFX(NA_SE_VO_BL_DOWN, 0x80, 2, 0, SFX_FLAG_10)
/* 0x681E */ DEFINE_SFX(NA_SE_VO_LI_DEMO_DAMAGE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681F */ DEFINE_SFX(NA_SE_VO_LI_ELECTRIC_SHOCK_LV, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6820 */ DEFINE_SFX(NA_SE_VO_LI_SWORD_N_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6821 */ DEFINE_SFX(NA_SE_VO_LI_ROLLING_CUT_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6822 */ DEFINE_SFX(NA_SE_VO_LI_LASH_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6823 */ DEFINE_SFX(NA_SE_VO_LI_HANG_KID, 0x20, 2, 0, SFX_FLAG_10)
/* 0x6824 */ DEFINE_SFX(NA_SE_VO_LI_CLIMB_END_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6825 */ DEFINE_SFX(NA_SE_VO_LI_DAMAGE_S_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6826 */ DEFINE_SFX(NA_SE_VO_LI_FREEZE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6827 */ DEFINE_SFX(NA_SE_VO_LI_FALL_S_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6828 */ DEFINE_SFX(NA_SE_VO_LI_FALL_L_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6829 */ DEFINE_SFX(NA_SE_VO_LI_BREATH_REST_KID, 0x30, 2, 1, SFX_FLAG_10)
/* 0x682A */ DEFINE_SFX(NA_SE_VO_LI_BREATH_DRINK_KID, 0x30, 2, 1, SFX_FLAG_10)
/* 0x682B */ DEFINE_SFX(NA_SE_VO_LI_DOWN_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x682C */ DEFINE_SFX(NA_SE_VO_LI_TAKEN_AWAY_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x682D */ DEFINE_SFX(NA_SE_VO_LI_HELD_KID, 0x50, 2, 0, SFX_FLAG_10)
/* 0x682E */ DEFINE_SFX(NA_SE_VO_LI_SNEEZE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x682F */ DEFINE_SFX(NA_SE_VO_LI_SWEAT_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6830 */ DEFINE_SFX(NA_SE_VO_LI_DRINK_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6831 */ DEFINE_SFX(NA_SE_VO_LI_RELAX_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6832 */ DEFINE_SFX(NA_SE_VO_LI_SWORD_PUTAWAY_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6833 */ DEFINE_SFX(NA_SE_VO_LI_GROAN_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6834 */ DEFINE_SFX(NA_SE_VO_LI_AUTO_JUMP_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6835 */ DEFINE_SFX(NA_SE_VO_LI_MAGIC_NALE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6836 */ DEFINE_SFX(NA_SE_VO_LI_SURPRISE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6837 */ DEFINE_SFX(NA_SE_VO_LI_MAGIC_FROL_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6838 */ DEFINE_SFX(NA_SE_VO_LI_PUSH_KID, 0x30, 1, 2, SFX_FLAG_10)
/* 0x6839 */ DEFINE_SFX(NA_SE_VO_LI_HOOKSHOT_HANG_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683A */ DEFINE_SFX(NA_SE_VO_LI_LAND_DAMAGE_S_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683B */ DEFINE_SFX(NA_SE_VO_LI_NULL_0x1b_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683C */ DEFINE_SFX(NA_SE_VO_LI_MAGIC_ATTACK_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683D */ DEFINE_SFX(NA_SE_VO_BL_DOWN_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683E */ DEFINE_SFX(NA_SE_VO_LI_DEMO_DAMAGE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683F */ DEFINE_SFX(NA_SE_VO_LI_ELECTRIC_SHOCK_LV_KID, 0x30, 2, 0, SFX_FLAG_10)

        <Effects>
            <Effect Name="Adult Link - Attack 1" Enum="LINK_ADULT_ATTACK_0"/>
            <Effect Name="Adult Link - Attack 2" Enum="LINK_ADULT_ATTACK_1"/>
            <Effect Name="Adult Link - Attack 3" Enum="LINK_ADULT_ATTACK_2"/>
            <Effect Name="Adult Link - Attack 4" Enum="LINK_ADULT_ATTACK_3"/>
            <Effect Name="Adult Link - Strong Attack 1" Enum="LINK_ADULT_MAJOR_0"/>
            <Effect Name="Adult Link - Strong Attack 2" Enum="LINK_ADULT_MAJOR_1"/>
            <Effect Name="Adult Link - Dangling Grunt" Enum="LINK_ADULT_LEDGE_0"/>
            <Effect Name="Adult Link - Climb Edge" Enum="LINK_ADULT_CLIMB"/>
            <Effect Name="Adult Link - Dangling Gasp 1" Enum="LINK_ADULT_LEDGE_1"/>
            <Effect Name="Adult Link - Hurt 1" Enum="LINK_ADULT_HURT_0"/>
            <Effect Name="Adult Link - Hurt 2" Enum="LINK_ADULT_HURT_1"/>
            <Effect Name="Adult Link - Hurt 3" Enum="LINK_ADULT_HURT_2"/>
            <Effect Name="Adult Link - Hurt 4" Enum="LINK_ADULT_HURT_3"/>
            <Effect Name="Adult Link - Knocked Back" Enum="LINK_ADULT_KNOCKBACK"/>
            <Effect Name="Adult Link - Hurt 5" Enum="LINK_ADULT_HURT_4"/>
            <Effect Name="Adult Link - Falling 1" Enum="LINK_ADULT_FALL_0"/>
            <Effect Name="Adult Link - Falling 2" Enum="LINK_ADULT_FALL_1"/>
            <Effect Name="Adult Link - Gasp 1" Enum="LINK_ADULT_GASP_0"/>
            <Effect Name="Adult Link - Gasp 2" Enum="LINK_ADULT_GASP_1"/>
            <Effect Name="Adult Link - Pant 1" Enum="LINK_ADULT_PANT_0"/>
            <Effect Name="Child Link - Dying Gasp" Enum="LINK_CHILD_DEATH_0"/>
            <Effect Name="Adult Link - Spur Horse 1" Enum="LINK_ADULT_SPUR_0"/>
            <Effect Name="Adult Link - Spur Horse 2" Enum="LINK_ADULT_SPUR_1"/>
            <Effect Name="Adult Link - Pant 2" Enum="LINK_ADULT_PANT_1"/>
            <Effect Name="Adult Link - Painful Landing" Enum="LINK_ADULT_FALL_DMG"/>
            <Effect Name="Adult Link - Dangling Gasp 2" Enum="LINK_ADULT_LEDGE_2"/>
            <Effect Name="Adult Link - Hup" Enum="LINK_ADULT_JUMP"/>
            <Effect Name="Adult Link - Gasp 3" Enum="LINK_ADULT_GASP_2"/>
            <Effect Name="Child Link - Attack 1" Enum="LINK_CHILD_ATTACK_0"/>
            <Effect Name="Child Link - Attack 2" Enum="LINK_CHILD_ATTACK_1"/>
            <Effect Name="Child Link - Attack 3" Enum="LINK_CHILD_ATTACK_2"/>
            <Effect Name="Child Link - Attack 4" Enum="LINK_CHILD_ATTACK_3"/>
            <Effect Name="Child Link - Strong Attack 1" Enum="LINK_CHILD_STRONG_0"/>
            <Effect Name="Child Link - Strong Attack 2" Enum="LINK_CHILD_STRONG_1"/>
            <Effect Name="Child Link - Dangling Gasp 1" Enum="LINK_CHILD_LEDGE_0"/>
            <Effect Name="Child Link - Sigh 1" Enum="LINK_CHILD_SIGH_0"/>
            <Effect Name="Child Link - Sigh 2" Enum="LINK_CHILD_SIGH_1"/>
            <Effect Name="Child Link - Hurt 1" Enum="LINK_CHILD_HURT_0"/>
            <Effect Name="Child Link - Hurt 2" Enum="LINK_CHILD_HURT_1"/>
            <Effect Name="Child Link - Hurt 3" Enum="LINK_CHILD_HURT_2"/>
            <Effect Name="Child Link - Hurt 4" Enum="LINK_CHILD_HURT_3"/>
            <Effect Name="Child Link - Knocked Back" Enum="LINK_CHILD_KNOCKBACK"/>
            <Effect Name="Child Link - Hurt 5" Enum="LINK_CHILD_HURT_4"/>
            <Effect Name="Child Link - Falling 1" Enum="LINK_CHILD_FALL_0"/>
            <Effect Name="Child Link - Falling 2" Enum="LINK_CHILD_FALL_1"/>
            <Effect Name="Child Link - Gasp 1" Enum="LINK_CHILD_GASP_0"/>
            <Effect Name="Child Link - Gasp 2" Enum="LINK_CHILD_GASP_1"/>
            <Effect Name="Child Link - Wheeze" Enum="LINK_CHILD_WHEEZE"/>
            <Effect Name="Child Link - Exhausted Panting" Enum="LINK_CHILD_PANT"/>
            <Effect Name="Child Link - Painful Landing" Enum="LINK_CHILD_FALL_DMG"/>
            <Effect Name="Child Link - Danging Gasp 2" Enum="LINK_CHILD_LEDGE_1"/>
            <Effect Name="Child Link - Glug" Enum="LINK_CHILD_DRINK"/>
            <Effect Name="Child Link - Refreshed" Enum="LINK_CHILD_REFRESH"/>
            <Effect Name="Child Link - Hup 1" Enum="LINK_CHILD_JUMP_0"/>
            <Effect Name="Child Link - Hup 2" Enum="LINK_ADULT_JUMP_1"/>
            <Effect Name="Adult Link - Glug" Enum="LINK_ADULT_DRINK"/>
            <Effect Name="Adult Link - Refreshed" Enum="LINK_ADULT_REFRESH"/>
            <Effect Name="Navi - Watch Out!" Enum="NAVI_WATCH_OUT"/>
            <Effect Name="Navi - Look!" Enum="NAVI_LOOK"/>
            <Effect Name="Navi - Hey!" Enum="NAVI_HEY"/>
            <Effect Name="Adult Link - Lift" Enum="LINK_ADULT_LIFT"/>
            <Effect Name="Adult Link - Cast Spell" Enum="LINK_ADULT_SPELL"/>
            <Effect Name="Child Link - Charge Up" Enum="LINK_CHILD_CHARGE"/>
            <Effect Name="Child Link - Cast Spell" Enum="LINK_CHILD_SPELL"/>
            <Effect Name="Child Link - Dying Gasp" Enum="LINK_CHILD_DEATH_1"/>
            <Effect Name="Child Link - Strangled" Enum="LINK_CHILD_STRANGLED"/>
            <Effect Name="Child Link - Gasp 3" Enum="LINK_CHILD_GASP_2"/>
            <Effect Name="Child Link - Shiver" Enum="LINK_CHILD_SHIVER"/>
            <Effect/>
            <Effect Name="Child Link - Sneeze" Enum="LINK_CHILD_SNEEZE"/>
            <Effect Name="Child Link - Grunt" Enum="LINK_CHILD_GRUNT"/>
            <Effect Name="Child Link - Moans From Heat" Enum="LINK_CHILD_HEAT_0"/>
            <Effect Name="Child Link - Awakening Grunt" Enum="LINK_CHILD_WAKE_0"/>
            <Effect Name="Child Link - Wiping Off Sweat" Enum="LINK_CHILD_HEAT_1"/>
            <Effect Name="Child Link - Yawn Starts" Enum="LINK_CHILD_WAKE_1"/>
            <Effect Name="Child Link - Yawn and Stretch" Enum="LINK_CHILD_WAKE_2"/>
            <Effect Name="Child Link - Stretch Refreshingly" Enum="LINK_CHILD_WAKE_3"/>
            <Effect Name="Adult Link - Hurt 6" Enum="LINK_ADULT_HURT_5"/>
            <Effect Name="Adult Link - Choking" Enum="LINK_ADULT_CHOKE"/>
            <Effect Name="Adult Link - Gasping" Enum="LINK_ADULT_GASPING"/>
            <Effect Name="Adult Link - Small Gasp" Enum="LINK_ADULT_GASP_3"/>
            <Effect Name="Adult Link - Unsettled Moan" Enum="LINK_ADULT_MOAN"/>
            <Effect Name="Adult Link - Sneeze" Enum="LINK_ADULT_SNEEZE"/>
            <Effect Name="Adult Link - Sigh 1" Enum="LINK_ADULT_SIGH_0"/>
            <Effect Name="Adult Link - Sigh 2" Enum="LINK_ADULT_SIGH_1"/>
            <Effect Name="Adult Link - Sigh 3" Enum="LINK_ADULT_SIGH_2"/>
            <Effect Name="Adult Link - Stretch Start" Enum="LINK_ADULT_STRETCH_0"/>
            <Effect Name="Adult Link - Stretching" Enum="LINK_ADULT_STRETCH_1"/>
            <Effect Name="Adult Link - Finished Stretching" Enum="LINK_ADULT_STRETCH_2"/>
            <Effect Name="Talon Snoring" Enum="TALON_SNORE"/>
            <Effect Name="Talon Startled" Enum="TALON_SURPRISE"/>
            <Effect Name="Talon Distressed" Enum="TALON_CURIOUS"/>
            <Effect Name="Talon Fearful" Enum="TALON_SCARED"/>
            <Effect Name="Ingo Yowls" Enum="INGO_MAD"/>
            <Effect Name="Ingo Exclaims" Enum="INGO_SHOUT"/>
            <Effect Name="Ingo Spurs Horse 1" Enum="INGO_SPUR_0"/>
            <Effect Name="Ingo Spurs Horse 2" Enum="INGO_SPUR_1"/>
            <Effect Name="Great Fairy Laughs" Enum="GREAT_FAIRY_LAUGH_0"/>
            <Effect Name="Great Fairy Titters" Enum="GREAT_FAIRY_LAUGH_1"/>
            <Effect Name="Zelda Gasps" Enum="ZELDA_GASP"/>
            <Effect Name="Girl Screams 1" Enum="GIRL_SCREAM_0"/>
            <Effect Name="Girl Inquiring 1" Enum="GIRL_INQUIRE_0"/>
            <Effect Name="Navi - Watch Out! 2" Enum="NAVI_WATCH_OUT_1"/>
            <Effect Name="Navi - Look! 2" Enum="NAVI_LOOK_1"/>
            <Effect Name="Navi - Hey! 2" Enum="NAVI_HEY_1"/>
            <Effect Name="Girl Hurt" Enum="GIRL_HURT"/>
            <Effect Name="Girl Startled 1" Enum="GIRL_SHOCK_0"/>
            <Effect Name="Girl Screams 2" Enum="GIRL_SCREAM_1"/>
            <Effect Name="Girl Delighted" Enum="GIRL_GIGGLE"/>
            <Effect Name="Girl Surprised 1" Enum="GIRL_SURPRISE_0"/>
            <Effect Name="Girl Surprised 2" Enum="GIRL_SURPRISE_1"/>
            <Effect Name="Girl Discombobulated" Enum="GIRL_UPSET"/>
            <Effect Name="Skulltula Screaming" Enum="SKULLTULA_UPSET"/>
            <Effect Name="Skulltula Grunts Angrily" Enum="SKULLTULA_ANGRY"/>
            <Effect Name="Girl Sighs" Enum="GIRL_SIGH_0"/>
            <Effect Name="Girl Startled 2" Enum="GIRL_SHOCK_1"/>
            <Effect Name="Girl Inquiring 2" Enum="GIRL_INQUIRE_1"/>
            <Effect Name="Girl Disappointed" Enum="GIRL_SIGH_1"/>
            <Effect Name="Girl Laughing" Enum="GIRL_LAUGH"/>
            <Effect Name="Girl Startled 3" Enum="GIRL_SHOCK_2"/>
            <Effect Name="Girl Grunts in Effort" Enum="GIRL_GRUNT"/>
            <Effect Name="Woman Yells" Enum="WOMAN_YELL_0"/>
            <Effect Name="Woman Startled, Yells" Enum="WOMAN_YELL_1"/>
            <Effect Name="Woman Grunts" Enum="WOMAN_GRUNT"/>
            <Effect Name="Navi - Listen!" Enum="NAVI_LISTEN"/>
            <Effect Name="Woman Hup" Enum="WOMAN_HUP"/>
            <Effect Name="Girl Screams 3" Enum="GIRL_SCREAM_2"/>
            <Effect Name="Girl Screams 4" Enum="GIRL_SCREAM_3"/>
            <Effect Name="Girl Sighs Contentedly" Enum="GIRL_SIGH_2"/>
            <Effect Name="Girl Surprised 3" Enum="GIRL_SHOCK_3"/>
            <Effect Name="Girl Scared" Enum="GIRL_SCARED"/>
            <Effect Name="King Zora Scoots" Enum="MWINK"/>
            <Effect Name="Navi - Hello!" Enum="NAVI_HELLO"/>
            <Effect/>
            <Effect Name="Adult Link - Dramatic Gasp" Enum="LINK_ADULT_GASP_4"/>
            <Effect Name="Child Link - Dramatic Gasp" Enum="LINK_CHILD_GASP_3"/>
        </Effects>

sfx_id_map = {

}

class VOICE_PACK_AGE(Enum):
    CHILD = 0
    ADULT = 1

def _patch_voice_pack(rom: Rom, age: VOICE_PACK_AGE, voice_pack: str):
    bank0 = rom.audiobanks[0]
    # Build voice pack path
    voice_pack_dir = os.path.join(data_path(), "Voices", "Child" if age == VOICE_PACK_AGE.CHILD else "Adult", voice_pack)

    sfx_list: list[tuple[str,int]] = child_link_sfx if age == VOICE_PACK_AGE.CHILD else adult_link_sfx

    # List all files in the directory
    files : list[str] = os.listdir(voice_pack_dir)

    sfxs = []
    for filename in files:
        if filename.startswith("00-") and filename.endswith(".aifc"): # Old style naming get bank and then sfx id from the file name
            substr = filename[0:7]
            split = substr.split("-")
            bank = split[0]
            sfxid = split[1]
            sfxs.append((os.path.join(voice_pack_dir,filename), filename, int(sfxid,16)))
        else: # New style naming, compare file name against table
            split = filename.split('.')
            filename_without_ext = split[0]
            ext = split[1]
            for sfx_name, sfx_id in sfx_list:
                if filename_without_ext == sfx_name and ext == "aifc":
                    sfxs.append((os.path.join(voice_pack_dir,filename), sfx_name, sfx_id))
                    break

    sfx_data_start = len(rom.audiotable)

    # Patch each sfx that we have
    for sfx_to_patch, sfx_name, sfx_id in sfxs:
        # Open the .aifc file
        index = 0
        f = open(sfx_to_patch, 'rb')
        # Read data from the .aifc file
        
        # Read the "FORM" Chunk
        f.read(4) # "FORM"
        size = int.from_bytes(f.read(4), 'big')
        type = str(f.read(4), encoding='utf-8')
        if type != "AIFC":
            raise Exception("Not an AIFC file")

        # Read the rest of the chunks
        done = False
        chunks = {}
        chkID = "FORM"
        while chkID != '':
            chkID = str(f.read(4), encoding='utf-8')
            size = int.from_bytes(f.read(4), 'big')
            data = f.read(size)
            chunk = {
                'size': size,
                'data': data
            }
            chunks[chkID] = chunk
        
        # Process the chunks
        
        # COMM Chunk: Sampling rate, compression type, number of channels
        #define CommonID 'COMM' /* ckID for Common Chunk */
        # typedef struct {
        #   ID ckID; /*  'COMM'  */
        #   long kDataSize;
        #   short numChannels; /* # audio channels */
        #   unsigned long numSampleFrames; /* # sample frames = samples/channel */
        #   short sampleSize; /* # bits/sample */
        #   extended sampleRate; /* sample_frames/sec */
        #   ID compressionType; /* compression type ID code */
        #   pstring compressionName; /* human-readable compression type name */
        # } CommonChunk;

        comm = chunks['COMM']
        data = comm['data']
        numChannels = int.from_bytes(data[0:2], 'big')
        numSampleFrames = int.from_bytes(data[2:6], 'big')
        sampleSize = int.from_bytes(data[6:8], 'big')
        sampleRateBytes = bytearray(data[8:18])
        # Need to process the sample rate. it's an 80-bit extended floating point value stored in 10 bytes which nothing natively supports
        # We can probably use NumPy if we put it in the proper byte order and extend to 128-bit
        dtype = np.dtype(np.longdouble)
        if sys.byteorder == 'little':
            sampleRateBytes.reverse()
        sampleRateBytes = sampleRateBytes + bytearray(16 - len(sampleRateBytes)) # Pad sampleRateBytes to 128 bit
        sampleRate = np.frombuffer(sampleRateBytes, dtype=dtype)[0]
        compressionType = str(data[18:22],encoding='utf-8')
        compressionNameLen = data[22]
        compressionName = str(data[23:23 + compressionNameLen], encoding='utf-8')
        
        # Make sure it's the correct compression type
        if compressionType != "ADP9":
            raise Exception("Unknown compression format. Must be 'ADP9'. Did you use vadpcm_enc?")
    
        # Make sure it's the correct sample size
        if sampleSize != 16:
            raise Exception("Unsupported sample size. Must be 16 bit samples")

        # Compressed sample data
        # SSND Chunk contains the sample data
        ssnd = chunks['SSND']
        data = ssnd['data']
        ssndOffset = int.from_bytes(data[0:4], 'big')
        ssndBlockSize = int.from_bytes(data[4:8], 'big')
        
        if ssndOffset != 0 or ssndBlockSize != 0:
            raise Exception("Unsupported SSND offset/block size")
        # Read the sample data. it's numSampleFrames * 9 / 8 / 2
        dataLen = int(ceil(numSampleFrames * 9 / 8 / 2))
        soundData = data[8:8 + dataLen]
        
        # Calculate the tuning as sampling rate / 32000.
        tuning = sampleRate / 32000

        # Pad the data to 16 bytes
        soundData += bytearray((16 - (len(soundData)%16))%16)

        # Sort-of problem. We need to update audiotable in multiple different spots. 
        # So instead of making the new file, maybe just add a new variable to Rom called new_audiotable_data and write it all at the end.
        # Update sample address to point to new data in audiotable.
        sfx: SFX = bank0.SFX[sfx_id]

        # Compare sound data to existing to see if it fits
        if len(soundData) > sfx.sample.size:
            # Put the data in audiotable
            rom.audiotable += soundData
            sfx.sample.addr = sfx_data_start
            sfx_data_start += len(soundData)
        else:
            # Put the data in the existing location. Pad with 0s
            pad_len = sfx.sample.size - len(soundData)
            soundData += bytearray(pad_len)
            rom.audiotable[sfx.sample.addr:sfx.sample.addr + len(soundData)] = soundData

        # Update the sfx tuning
        sfx.sampleTuning = float(tuning)

        # Update loop end as numSampleFrames
        sfx.sample.loop.end = numSampleFrames
        # Update sample data length = length
        sfx.sample.size = dataLen
        sampleBytes = sfx.sample.get_bytes()
        sfxBytes = sfx.get_bytes()
        loopBytes = sfx.sample.loop.get_bytes()

        # Write the new sample into the bank
        bank0.bank_data[sfx.sampleOffset:sfx.sampleOffset+0x10] = sfx.sample.get_bytes()
        bank0.bank_data[sfx.sfx_offset:sfx.sfx_offset+0x08] = sfx.get_bytes()
        bank0.bank_data[sfx.sample.loop_addr:sfx.sample.loop_addr+len(loopBytes)] = loopBytes

def rename_old_files(dir: str, age: VOICE_PACK_AGE):
    # list the contents of the directory

    files : list[str] = os.listdir(dir)
    sfxlist = adult_link_sfx if age == VOICE_PACK_AGE.ADULT else child_link_sfx
    for file in files:
        if file.startswith("00-") and (file.endswith(".aifc")):
            # Rename this file
            split = file.split("_")
            split = split[0].split("-")
            bank = split[0]
            oldsfxid = int(split[1],16)
            # Get the name from the table
            for sfxname, sfxid in sfxlist:
                # Rename the file to sfxname
                if oldsfxid == sfxid:
                    old_path = os.path.join(dir,file)
                    new_path = os.path.join(dir,sfxname+ ".aifc")
                    print("Renaming " + old_path)
                    os.rename(old_path, new_path)
                    break

if __name__ == "__main__":
    rom = Rom("ZOOTDEC.z64")
    rename_old_files("data/Voices/Child/Feminine_New", VOICE_PACK_AGE.CHILD)
    #_patch_voice_pack(rom, "Mario", VOICE_PACK_AGE.ADULT)

