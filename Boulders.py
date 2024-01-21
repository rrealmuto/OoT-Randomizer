# Literally the stupidest idea ever and I love it

from Rom import *
from ProcessActors import get_actor_list, scenes
import random
from enum import IntEnum

class BOULDER_TYPE(IntEnum):
    BROWN = 0
    BRONZE = 1
    SILVER = 2
    RED_ICE = 3
    HEAVY_BLOCK = 4

boulder_rules = {
    'BOULDER_TYPE_BROWN': int(BOULDER_TYPE.BROWN),
    'BOULDER_TYPE_BRONZE': int(BOULDER_TYPE.BRONZE),
    'BOULDER_TYPE_SILVER': int(BOULDER_TYPE.SILVER),
    'BOULDER_TYPE_RED_ICE': int(BOULDER_TYPE.RED_ICE),
    'BOULDER_TYPE_HEAVY_BLOCK': int(BOULDER_TYPE.HEAVY_BLOCK)
}

vanilla_dungeon_boulders = {
    'Bottom of the Well': {
        'BOTW_CHEST_BOULDER_1': { (8, 1, 0, 32): {'type': BOULDER_TYPE.BROWN, 'switch': 12, },},
        'BOTW_CHEST_BOULDER_2': { (8, 1, 0, 33): {'type': BOULDER_TYPE.BROWN, 'switch': 19, },},
        'BOTW_UNNAMED_1': {(8, 1, 0, 34): {'type': BOULDER_TYPE.BROWN, 'switch': 20, },},
        'BOTW_UNNAMED_2': {(8, 1, 0, 35): {'type': BOULDER_TYPE.BROWN, 'switch': 21, },},
        'BOTW_GRASS_BOULDER_1': {(8, 1, 0, 36): {'type': BOULDER_TYPE.BROWN, 'switch': 25, },},
        'BOTW_GRASS_BOULDER_2': {(8, 1, 0, 37): {'type': BOULDER_TYPE.BROWN, 'switch': 26, },},
    },
    'Ice Cavern': {
        'ICE_CAVERN_UNNAMED_1': {(9, 0, 0, 5): {'type': BOULDER_TYPE.RED_ICE, 'switch': 0, },},
        'ICE_CAVERN_FREESTANDING_COLLECTIBLE_BOULDER': {(9, 1, 0, 8): {'type': BOULDER_TYPE.RED_ICE, 'switch': 3, },},
        'ICE_CAVERN_UNNAMED_2': {(9, 1, 0, 9): {'type': BOULDER_TYPE.RED_ICE, 'switch': 4, },},
        'ICE_CAVERN_TOWARDS_COMPASS_BOULDER': {(9, 3, 0, 17): {'type': BOULDER_TYPE.RED_ICE, 'switch': 12, },},
        'ICE_CAVERN_TOWARDS_BLOCK_PUSH_BOULDER': {(9, 3, 0, 18): {'type': BOULDER_TYPE.RED_ICE, 'switch': 13, },},
        'ICE_CAVERN_BLOCK_PUSH_SILVER_RUPEE_BOULDER': {(9, 5, 0, 19): {'type': BOULDER_TYPE.RED_ICE, 'switch': 16, },},
        'ICE_CAVERN_NEAR_END_BOULDER_1': {(9, 6, 0, 14): {'type': BOULDER_TYPE.RED_ICE, 'switch': 17, },},
        'ICE_CAVERN_NEAR_END_BOULDER_2': {(9, 6, 0, 15): {'type': BOULDER_TYPE.RED_ICE, 'switch': 18, },},
        'ICE_CAVERN_NEAR_END_BOULDER_3': {(9, 6, 0, 16): {'type': BOULDER_TYPE.RED_ICE, 'switch': 19, },},
        'ICE_CAVERN_NEAR_MAP_POT_BOULDER': {(9, 9, 0, 11): {'type': BOULDER_TYPE.RED_ICE, 'switch': 6, },},
        'ICE_CAVERN_MAP_BOULDER': {(9, 9, 0, 12): {'type': BOULDER_TYPE.RED_ICE, 'switch': 7, },},
        'ICE_CAVERN_COMPASS_CHEST_BOULDER': {(9, 11, 0, 26): {'type': BOULDER_TYPE.RED_ICE, 'switch': 14, },},
        'ICE_CAVERN_FREESTANDING_POH_BOULDER': {(9, 11, 0, 27): {'type': BOULDER_TYPE.RED_ICE, 'switch': 15, },},
    },
    'Ganons Castle': {
        'IGC_WATER_TRIAL_BOULDER_1': {(13, 2, 0, 28): {'type': BOULDER_TYPE.RED_ICE, 'switch': 6, },},
        'IGC_WATER_TRIAL_BOULDER_2': {(13, 3, 0, 5): {'type': BOULDER_TYPE.RED_ICE, 'switch': 39, },},
    }
}

mq_dungeon_boulders = {

}

boulder_list = {
    
    'HF_UNNAMED_1': {(81, 0, 1, 27): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (81, 0, 2, 37): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (81, 0, 0, 30): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'HF_FAIRY_GROTTO_BOULDER': {(81, 0, 0, 68): {'type': BOULDER_TYPE.BROWN, 'switch': 8, }, (81, 0, 1, 68): {'type': BOULDER_TYPE.BROWN, 'switch': 8, }, (81, 0, 2, 67): {'type': BOULDER_TYPE.BROWN, 'switch': 8, },},
    'HF_NEAR_MARKET_GROTTO_BOULDER': {(81, 0, 0, 69): {'type': BOULDER_TYPE.BROWN, 'switch': 14, }, (81, 0, 1, 67): {'type': BOULDER_TYPE.BROWN, 'switch': 14, }, (81, 0, 2, 68): {'type': BOULDER_TYPE.BROWN, 'switch': 14, },},
    'HF_SOUTHEAST_GROTTO_BOULDER': {(81, 0, 0, 70): {'type': BOULDER_TYPE.BROWN, 'switch': 16, }, (81, 0, 1, 66): {'type': BOULDER_TYPE.BROWN, 'switch': 16, }, (81, 0, 2, 69): {'type': BOULDER_TYPE.BROWN, 'switch': 16, },},
    'HF_COW_GROTTO_ADULT_BOULDER': {(81, 0, 2, 8): {'type': BOULDER_TYPE.BRONZE, 'switch': 17, },},
    'HF_UNNAMED_2': {(81, 0, 2, 9): {'type': BOULDER_TYPE.BRONZE, 'switch': 18, },},
    'HF_UNNAMED_3': {(81, 0, 2, 10): {'type': BOULDER_TYPE.BRONZE, 'switch': 20, },},
    'HF_UNNAMED_4': {(81, 0, 2, 11): {'type': BOULDER_TYPE.BRONZE, 'switch': 21, },},
    'HF_UNNAMED_5': {(82, 0, 2, 48): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (82, 0, 3, 19): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'ZR_CHILD_ENTRY_BOULDER_1': {(84, 0, 0, 60): {'type': BOULDER_TYPE.BROWN, 'switch': 2, },},
    'ZR_CHILD_ENTRY_BOULDER_2': {(84, 0, 0, 62): {'type': BOULDER_TYPE.BROWN, 'switch': 8, },},
    'ZR_CHILD_ENTRY_BOULDER_3': {(84, 0, 0, 63): {'type': BOULDER_TYPE.BROWN, 'switch': 9, },},
    'ZR_FAIRY_GROTTO_BOULDER': {(84, 0, 0, 61): {'type': BOULDER_TYPE.BROWN, 'switch': 5, }, (84, 0, 2, 27): {'type': BOULDER_TYPE.BROWN, 'switch': 5, },},
    'HF_UNNAMED_6': {(84, 0, 0, 64): {'type': BOULDER_TYPE.BROWN, 'switch': 10, },},
    'ZD_ADULT_SHOP_BOULDER': {(88, 1, 2, 15): {'type': BOULDER_TYPE.RED_ICE, 'switch': 4, },},
    'ZF_SECRET_SILVER_BOULDER': {(89, 0, 0, 5): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (89, 0, 1, 5): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (89, 0, 2, 48): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'ZF_UNNAMED_1': {(89, 0, 0, 16): {'type': BOULDER_TYPE.BROWN, 'switch': 21, }, (89, 0, 1, 16): {'type': BOULDER_TYPE.BROWN, 'switch': 21, }, (89, 0, 2, 52): {'type': BOULDER_TYPE.BROWN, 'switch': 21, },},
    'ZF_SECRET_BROWN_BOULDER': {(89, 0, 0, 17): {'type': BOULDER_TYPE.BROWN, 'switch': 22, }, (89, 0, 1, 17): {'type': BOULDER_TYPE.BROWN, 'switch': 22, }, (89, 0, 2, 53): {'type': BOULDER_TYPE.BROWN, 'switch': 22, },},
    'GV_CHEST_BOULDER_1': {(90, 0, 2, 15): {'type': BOULDER_TYPE.BRONZE, 'switch': 11, },},
    'GV_UNNAMED_2': {(90, 0, 2, 16): {'type': BOULDER_TYPE.BRONZE, 'switch': 4, },},
    'GV_UNNAMED_3': {(90, 0, 2, 17): {'type': BOULDER_TYPE.BRONZE, 'switch': 5, },},
    'GV_UNNAMED_4': {(90, 0, 2, 18): {'type': BOULDER_TYPE.BRONZE, 'switch': 6, },},
    'GV_UNNAMED_5': {(90, 0, 2, 19): {'type': BOULDER_TYPE.BRONZE, 'switch': 7, },},
    'GV_CHEST_BOULDER_2': {(90, 0, 2, 20): {'type': BOULDER_TYPE.BRONZE, 'switch': 9, },},
    'GV_CHEST_BOULDER_3': {(90, 0, 2, 21): {'type': BOULDER_TYPE.BRONZE, 'switch': 13, },},
    'GV_CHEST_BOULDER_4': {(90, 0, 2, 22): {'type': BOULDER_TYPE.BRONZE, 'switch': 16, },},
    'GV_UNNAMED_9': {(90, 0, 2, 27): {'type': BOULDER_TYPE.BROWN, 'switch': 10, },},
    'GV_UNNAMED_10': {(90, 0, 2, 28): {'type': BOULDER_TYPE.BROWN, 'switch': 12, },},
    'GV_UNNAMED_11': {(90, 0, 2, 29): {'type': BOULDER_TYPE.BROWN, 'switch': 15, },},
    'GV_OCTOROK_GROTTO_BOULDER': {(90, 0, 0, 18): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (90, 0, 2, 25): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'LW_NEAR_SHORTCUT_BOULDER': {(91, 2, 0, 6): {'type': BOULDER_TYPE.BROWN, 'switch': 17, }, (91, 2, 2, 7): {'type': BOULDER_TYPE.BROWN, 'switch': 17, },},
    'LW_RUPEE_BOULDER': {(91, 7, 0, 5): {'type': BOULDER_TYPE.BROWN, 'switch': 30, }, (91, 7, 2, 5): {'type': BOULDER_TYPE.BROWN, 'switch': 30, },},
    'LW_SCRUBS_GROTTO_BOULDER': {(91, 8, 0, 8): {'type': BOULDER_TYPE.BROWN, 'switch': 31, }, (91, 8, 2, 7): {'type': BOULDER_TYPE.BROWN, 'switch': 31, },},
    'COLOSSUS_GROTTO_BOULDER': {(92, 0, 0, 18): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (92, 0, 2, 21): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'HC_CHILD_FAIRY_BOULDER': {(95, 0, 0, 58): {'type': BOULDER_TYPE.BROWN, 'switch': 1, },},
    'DMT_ADULT_GC_SKULLTULA_BOULDER': {(96, 0, 2, 10): {'type': BOULDER_TYPE.BRONZE, 'switch': 9, },},
    'DMT_UNNAMED_1': {(96, 0, 2, 11): {'type': BOULDER_TYPE.BRONZE, 'switch': 20, },},
    'DMT_UNNAMED_2': {(96, 0, 2, 12): {'type': BOULDER_TYPE.BRONZE, 'switch': 21, },},
    'DMT_UNNAMED_3': {(96, 0, 2, 13): {'type': BOULDER_TYPE.BRONZE, 'switch': 22, },},
    'DMT_UNNAMED_4': {(96, 0, 2, 14): {'type': BOULDER_TYPE.BRONZE, 'switch': 23, },},
    'DMT_UNNAMED_5': {(96, 0, 2, 15): {'type': BOULDER_TYPE.BRONZE, 'switch': 24, },},
    'DMT_UNNAMED_6': {(96, 0, 2, 16): {'type': BOULDER_TYPE.BRONZE, 'switch': 25, },},
    'DMT_ADULT_SUMMIT_SKULLTULA_BOULDER': {(96, 0, 2, 17): {'type': BOULDER_TYPE.BRONZE, 'switch': 26, },},
    'DMT_UNNAMED_7': {(96, 0, 2, 18): {'type': BOULDER_TYPE.BRONZE, 'switch': 27, },},
    'DMT_UNNAMED_8': {(96, 0, 2, 19): {'type': BOULDER_TYPE.BRONZE, 'switch': 28, },},
    'DMT_UNNAMED_9': {(96, 0, 2, 20): {'type': BOULDER_TYPE.BRONZE, 'switch': 29, },},
    'DMT_CHILD_LOWER_BOULDER': {(96, 0, 0, 47): {'type': BOULDER_TYPE.BROWN, 'switch': 7, },},
    'DMT_MIDDLE_BOULDER': {(96, 0, 0, 48): {'type': BOULDER_TYPE.BROWN, 'switch': 8, }, (96, 0, 2, 48): {'type': BOULDER_TYPE.BROWN, 'switch': 8, },},
    'DMT_UPPER_BOULDER': {(96, 0, 0, 49): {'type': BOULDER_TYPE.BROWN, 'switch': 10, }, (96, 0, 2, 49): {'type': BOULDER_TYPE.BROWN, 'switch': 10, },},
    'DMT_COW_GROTTO_BOULDER': {(96, 0, 0, 50): {'type': BOULDER_TYPE.BROWN, 'switch': 31, }, (96, 0, 2, 50): {'type': BOULDER_TYPE.BROWN, 'switch': 31, },},
    'DMC_UNNAMED_1': {(97, 1, 2, 45): {'type': BOULDER_TYPE.BROWN, 'switch': 30, },},
    'DMC_UNNAMED_2': {(97, 1, 2, 46): {'type': BOULDER_TYPE.BROWN, 'switch': 31, },},
    'DMC_SCRUB_GROTTO_BOULDER': {(97, 1, 0, 11): {'type': BOULDER_TYPE.BRONZE, 'switch': 63, }, (97, 1, 2, 6): {'type': BOULDER_TYPE.BRONZE, 'switch': 23, },},
    'DMC_FAIRY_BOULDER_1': {(97, 1, 0, 12): {'type': BOULDER_TYPE.BRONZE, 'switch': 63, }, (97, 1, 2, 4): {'type': BOULDER_TYPE.BRONZE, 'switch': 21, },},
    'DMC_FAIRY_BOULDER_2': {(97, 1, 0, 13): {'type': BOULDER_TYPE.BRONZE, 'switch': 63, }, (97, 1, 2, 5): {'type': BOULDER_TYPE.BRONZE, 'switch': 22, },},
    'DMC_BOULDER_NEAR_FAIRY': {(97, 1, 0, 14): {'type': BOULDER_TYPE.BRONZE, 'switch': 63, }, (97, 1, 2, 7): {'type': BOULDER_TYPE.BRONZE, 'switch': 24, },},
    'DMC_CHILD_FIRE_TEMPLE_BOULDER_1': {(97, 1, 0, 27): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'DMC_CHILD_FIRE_TEMPLE_BOULDER_2': {(97, 1, 0, 28): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'DMC_CHILD_FIRE_TEMPLE_BOULDER_3': {(97, 1, 0, 29): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'DMC_UPPER_GROTTO_BOULDER': {(97, 1, 0, 35): {'type': BOULDER_TYPE.BROWN, 'switch': 29, }, (97, 1, 2, 47): {'type': BOULDER_TYPE.BROWN, 'switch': 29, },},
    'DMC_UNNAMED_3': {(97, 1, 0, 36): {'type': BOULDER_TYPE.BROWN, 'switch': 30, },},
    'DMC_UNNAMED_4': {(97, 1, 0, 37): {'type': BOULDER_TYPE.BROWN, 'switch': 31, },},
    'GC_BOULDER_MAZE_1': {(98, 0, 0, 0): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 6): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_2': {(98, 0, 0, 1): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 7): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_3': {(98, 0, 0, 2): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 8): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_4': {(98, 0, 0, 3): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 9): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_5': {(98, 0, 0, 4): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 10): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_6': {(98, 0, 0, 5): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 11): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_7': {(98, 0, 0, 6): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 12): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_8': {(98, 0, 0, 7): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 13): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_9': {(98, 0, 0, 8): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 14): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_10': {(98, 0, 0, 9): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },  (98, 0, 2, 15): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_11': {(98, 0, 0, 10): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 16): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_12': {(98, 0, 0, 11): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 17): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_13': {(98, 0, 0, 12): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 18): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_14': {(98, 0, 0, 13): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 19): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_15': {(98, 0, 0, 14): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 20): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_16': {(98, 0, 0, 15): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 21): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_17': {(98, 0, 0, 16): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 22): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_18': {(98, 0, 0, 17): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 23): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_19': {(98, 0, 0, 18): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 24): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_20': {(98, 0, 0, 19): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 25): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_21': {(98, 0, 0, 20): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 26): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_22': {(98, 0, 0, 21): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 27): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_23': {(98, 0, 0, 22): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 28): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_24': {(98, 0, 0, 23): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 29): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_25': {(98, 0, 0, 24): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 30): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_26': {(98, 0, 0, 25): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 31): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_27': {(98, 0, 0, 26): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 32): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_28': {(98, 0, 0, 27): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 33): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_29': {(98, 0, 0, 31): {'type': BOULDER_TYPE.BROWN, 'switch': 32, }, (98, 0, 2, 36): {'type': BOULDER_TYPE.BROWN, 'switch': 32, },},
    'GC_BOULDER_MAZE_30': {(98, 0, 0, 32): {'type': BOULDER_TYPE.BROWN, 'switch': 33, }, (98, 0, 2, 37): {'type': BOULDER_TYPE.BROWN, 'switch': 33, },},
    'GC_BOULDER_MAZE_31': {(98, 0, 0, 33): {'type': BOULDER_TYPE.BROWN, 'switch': 34, }, (98, 0, 2, 38): {'type': BOULDER_TYPE.BROWN, 'switch': 34, },},
    'GC_BOULDER_MAZE_32': {(98, 0, 0, 34): {'type': BOULDER_TYPE.BROWN, 'switch': 35, }, (98, 0, 2, 39): {'type': BOULDER_TYPE.BROWN, 'switch': 35, },},
    'GC_BOULDER_MAZE_33': {(98, 0, 0, 35): {'type': BOULDER_TYPE.BROWN, 'switch': 36, }, (98, 0, 2, 40): {'type': BOULDER_TYPE.BROWN, 'switch': 36, },},
    'GC_BOULDER_MAZE_34': {(98, 0, 0, 36): {'type': BOULDER_TYPE.BROWN, 'switch': 37, }, (98, 0, 2, 41): {'type': BOULDER_TYPE.BROWN, 'switch': 37, },},
    'GC_BOULDER_MAZE_35': {(98, 0, 0, 37): {'type': BOULDER_TYPE.BROWN, 'switch': 38, }, (98, 0, 2, 42): {'type': BOULDER_TYPE.BROWN, 'switch': 38, },},
    'GC_BOULDER_MAZE_36': {(98, 0, 0, 38): {'type': BOULDER_TYPE.BROWN, 'switch': 39, }, (98, 0, 2, 43): {'type': BOULDER_TYPE.BROWN, 'switch': 39, },},
    'GC_BOULDER_MAZE_37': {(98, 0, 0, 39): {'type': BOULDER_TYPE.BROWN, 'switch': 40, }, (98, 0, 2, 44): {'type': BOULDER_TYPE.BROWN, 'switch': 40, },},
    'GC_BOULDER_MAZE_38': {(98, 0, 0, 40): {'type': BOULDER_TYPE.BROWN, 'switch': 41, }, (98, 0, 2, 45): {'type': BOULDER_TYPE.BROWN, 'switch': 41, },},
    'GC_BOULDER_MAZE_39': {(98, 0, 0, 41): {'type': BOULDER_TYPE.BRONZE, 'switch': 42, }, (98, 0, 2, 1): {'type': BOULDER_TYPE.BRONZE, 'switch': 42, },},
    'GC_BOULDER_MAZE_40': {(98, 0, 0, 42): {'type': BOULDER_TYPE.BRONZE, 'switch': 43, }, (98, 0, 2, 2): {'type': BOULDER_TYPE.BRONZE, 'switch': 43, },},
    'GC_BOULDER_MAZE_41': {(98, 0, 0, 43): {'type': BOULDER_TYPE.BRONZE, 'switch': 44, }, (98, 0, 2, 3): {'type': BOULDER_TYPE.BRONZE, 'switch': 44, },},
    'GC_BOULDER_MAZE_42': {(98, 0, 0, 44): {'type': BOULDER_TYPE.SILVER, 'switch': 60, }, (98, 0, 2, 34): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'GC_BOULDER_MAZE_43': {(98, 0, 0, 45): {'type': BOULDER_TYPE.BRONZE, 'switch': 47, }, (98, 0, 2, 4): {'type': BOULDER_TYPE.BRONZE, 'switch': 47, },},
    'GC_BOULDER_MAZE_44': {(98, 0, 0, 46): {'type': BOULDER_TYPE.BRONZE, 'switch': 48, }, (98, 0, 2, 5): {'type': BOULDER_TYPE.BRONZE, 'switch': 48, },},
    'GC_UPPER_BOULDER_1': {(98, 3, 0, 22): {'type': BOULDER_TYPE.BROWN, 'switch': 3, },  (98, 3, 2, 29): {'type': BOULDER_TYPE.BROWN, 'switch': 3, },},
    'GC_UPPER_BOULDER_2': {(98, 3, 0, 25): {'type': BOULDER_TYPE.BROWN, 'switch': 9, },  (98, 3, 2, 31): {'type': BOULDER_TYPE.BROWN, 'switch': 9, },},
    'GC_UPPER_BOULDER_3': {(98, 3, 0, 26): {'type': BOULDER_TYPE.BROWN, 'switch': 10, },  (98, 3, 2, 32): {'type': BOULDER_TYPE.BROWN, 'switch': 10, },},
    'GC_LW_BOULDER_1': {(98, 3, 0, 24): {'type': BOULDER_TYPE.BROWN, 'switch': 8, },  (98, 3, 2, 30): {'type': BOULDER_TYPE.BROWN, 'switch': 8, },},
    'GC_LW_BOULDER_2': {(98, 3, 0, 27): {'type': BOULDER_TYPE.BROWN, 'switch': 11, },  (98, 3, 2, 33): {'type': BOULDER_TYPE.BROWN, 'switch': 11, },},
    'GC_LW_BOULDER_3': {(98, 3, 0, 28): {'type': BOULDER_TYPE.BROWN, 'switch': 12, },  (98, 3, 2, 34): {'type': BOULDER_TYPE.BROWN, 'switch': 12, },},
    'OGC_UNNAMED_1': {(100, 0, 0, 3): {'type': BOULDER_TYPE.BRONZE, 'switch': 3, },},
    'OGC_UNNAMED_2': {(100, 0, 0, 4): {'type': BOULDER_TYPE.BRONZE, 'switch': 5, },},
    'OGC_UNNAMED_3': {(100, 0, 0, 5): {'type': BOULDER_TYPE.BRONZE, 'switch': 6, },},
    'OGC_UNNAMED_4': {(100, 0, 0, 6): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'OGC_UNNAMED_5': {(100, 0, 0, 7): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'OGC_UNNAMED_6': {(100, 0, 0, 8): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
    'OGC_UNNAMED_7': {(100, 0, 0, 9): {'type': BOULDER_TYPE.SILVER, 'switch': 60, },},
}

def process_brown_boulder(actor_bytes):
    return {
        "type": BOULDER_TYPE.BROWN,
        "switch": (actor_bytes[15] & 0x3F)
    }

def process_bronze_boulder(actor_bytes):
    return {
        "type": BOULDER_TYPE.BRONZE,
        "switch": (actor_bytes[15] & 0x3F)
    }

def process_silver_boulder(actor_bytes):
    var = (actor_bytes[14] << 8) | (actor_bytes[15])
    return {
        "type": BOULDER_TYPE.SILVER,
        "switch": ((var >> 0xA) & 0x3C) | ((var >> 6) & 3)
    }

def process_red_ice(actor_bytes):
    return {
        "type": BOULDER_TYPE.RED_ICE,
        "switch": (actor_bytes[15] & 0x3F)
    }

def process_heavyblock(actor_bytes):
    return {
        "type": BOULDER_TYPE.HEAVY_BLOCK,
        "switch": (actor_bytes[14] & 0x3F)
    }

def get_boulder_shuffle_actors(rom):
    def get_boulder_shuffle_actors_func(rom, actor_id, actor, scene, room_id, setup_num, actor_num):
        if actor_id == 0x0127: # Brown bombable boulder
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_brown_boulder(rom.read_bytes(actor, 16)))
        elif actor_id == 0x01D2: #Bronze (hammer) boulder
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_bronze_boulder(rom.read_bytes(actor, 16)))
        elif actor_id == 0x014E: #Silver rock (str2)
            actor_bytes = rom.read_bytes(actor, 16)
            if actor_bytes[15] & 0x0001: # is a big rock and not a little one
                return (scene, room_id, setup_num, actor_num, scenes[scene], process_silver_boulder(rom.read_bytes(actor, 16)))
        #elif actor_id == 0x0092: #Heavy Block (str3 pillar)
        #    return (scene, room_id, setup_num, actor_num, scenes[scene], process_heavyblock(rom.read_bytes(actor, 16)))
        elif actor_id == 0x00EF: # Red Ice
            return (scene, room_id, setup_num, actor_num, scenes[scene], process_red_ice(rom.read_bytes(actor, 16)))
    return get_actor_list(rom, get_boulder_shuffle_actors_func)

def convert_brown_boulder(actor):
    return (0x0127, actor['switch'] & 0x3F)
def convert_bronze_boulder(actor):
    return (0x01D2, actor['switch'] & 0x3F)
def convert_silver_boulder(actor):
    switch = actor['switch']
    var = ((switch & 0x3C) << 0xA) | ((switch & 3) << 6)
    var = var | 0x01
    return  (0x014E, var)
def convert_red_ice(actor):
    return (0x00EF, actor['switch'])
def convert_heavyblock(actor):
    return

convert = {
    BOULDER_TYPE.BROWN: convert_brown_boulder,
    BOULDER_TYPE.BRONZE: convert_bronze_boulder,
    BOULDER_TYPE.SILVER: convert_silver_boulder,
    BOULDER_TYPE.RED_ICE: convert_red_ice,
    BOULDER_TYPE.HEAVY_BLOCK: convert_heavyblock
}

def shuffle_boulders(world) -> tuple[dict[str, dict[tuple[int,int,int,int], dict[str, any]]], dict[tuple[int,int,int,int], BOULDER_TYPE]]:
    # Build the full boulder list from the overworld ones + dungeons
    boulders = boulder_list

    mq_dungeons = [dungeon for dungeon in world.dungeon_mq if world.dungeon_mq[dungeon] == True]
    vanilla_dungeons = [dungeon for dungeon in world.dungeon_mq if world.dungeon_mq[dungeon] == False]

    for dungeon in mq_dungeons:
        if dungeon in mq_dungeon_boulders.keys():
            boulders.update(mq_dungeon_boulders[dungeon])
    
    for dungeon in vanilla_dungeons:
        if dungeon in vanilla_dungeon_boulders.keys():
            boulders.update(vanilla_dungeon_boulders[dungeon])

    # Get each boulder's type which we'll shuffle
    target_types = [boulders[boulder][list(boulders[boulder])[0]]['type'] for boulder in boulders]
    if world.settings.shuffle_boulders:
        random.shuffle(target_types)
    shuffled_boulders_by_id = {}
    shuffled_boulders = {}
    boulder_keys = list(boulders)
    for i in range(0,len(boulders)):
        boulder_key = boulder_keys[i]
        boulder = boulders[boulder_key] # Contains a dict of boulders for this because we have multiple scene setups
        shuffled_boulders [boulder_key] = target_types[i]
        for id in boulder:
            shuffled_boulders_by_id[(id[0], id[1], id[2], id[3])] = target_types[i]

    return shuffled_boulders, shuffled_boulders_by_id

def patch_boulders(boulders: dict[tuple[int,int,int,int], BOULDER_TYPE], rom: Rom):
    boulder_rom_actors = get_boulder_shuffle_actors(rom)
    rom_boulders = {(boulder_rom_actors[addr][0],boulder_rom_actors[addr][1],boulder_rom_actors[addr][2],boulder_rom_actors[addr][3]): {'addr': addr, 'type':boulder_rom_actors[addr][5]['type'], 'switch':boulder_rom_actors[addr][5]['switch']} for addr in list(boulder_rom_actors.keys())}
    for key in boulders:
        new_type = boulders[key]
        rom_actor = rom_boulders[key]
        if new_type != rom_actor['type']:
            (id, var) = convert[new_type](rom_actor)
            rom.write_int16(rom_actor['addr'], id)
            rom.write_int16(rom_actor['addr'] + 14, var)


#rom = Rom("ZOOTDEC.z64")
#boulder_rom_actors = get_boulder_shuffle_actors(rom)
#for actor in boulder_rom_actors:
#    print(boulder_rom_actors[actor])
#boulders = shuffle_boulders()
#patch_boulders(boulders, rom)