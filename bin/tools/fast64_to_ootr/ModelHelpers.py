
from enum import IntEnum

# Misc. constants
CODE_START: int          = 0x00A87000
PLAYER_START: int        = 0x00BCDB70
HOOK_START: int          = 0x00CAD2C0
SHIELD_START: int        = 0x00DB1F40
STICK_START: int         = 0x00EAD0F0
GRAVEYARD_KID_START: int = 0x00E60920
GUARD_START: int         = 0x00D1A690
RUNNING_MAN_START: int   = 0x00E50440

BASE_OFFSET: int         = 0x06000000
LUT_START: int           = 0x00005000
LUT_END: int             = 0x00005800
PRE_CONSTANT_START: int  = 0X0000500C

ADULT_START: int         = 0x00F86000
ADULT_SIZE: int          = 0x00037800
ADULT_OBJ_TABLE_ENTRY    = 0x00B6EFF8
ADULT_POST_START: int    = 0x00005238

CHILD_START: int         = 0x00FBE000
CHILD_SIZE: int          = 0x0002CF80
CHILD_HIERARCHY: int     = 0x060053A8
CHILD_POST_START: int    = 0x00005228
CHILD_OBJ_TABLE_ENTRY    = 0x00B6F000


class LUT:
    def __init__(self, lut_base: int):
        self.base = lut_base
    
    def offset(self, offset: int):
        return self.base + offset

# LUT offsets for adult and child
class Offsets(IntEnum):
    ADULT_LINK_LUT_DL_WAIST = 0x0090
    ADULT_LINK_LUT_DL_RTHIGH = 0x0098
    ADULT_LINK_LUT_DL_RSHIN = 0x00A0
    ADULT_LINK_LUT_DL_RFOOT = 0x00A8
    ADULT_LINK_LUT_DL_LTHIGH = 0x00B0
    ADULT_LINK_LUT_DL_LSHIN = 0x00B8
    ADULT_LINK_LUT_DL_LFOOT = 0x00C0
    ADULT_LINK_LUT_DL_HEAD = 0x00C8
    ADULT_LINK_LUT_DL_HAT = 0x00D0
    ADULT_LINK_LUT_DL_COLLAR = 0x00D8
    ADULT_LINK_LUT_DL_LSHOULDER = 0x00E0
    ADULT_LINK_LUT_DL_LFOREARM = 0x00E8
    ADULT_LINK_LUT_DL_RSHOULDER = 0x00F0
    ADULT_LINK_LUT_DL_RFOREARM = 0x00F8
    ADULT_LINK_LUT_DL_TORSO = 0x0100
    ADULT_LINK_LUT_DL_LHAND = 0x0108
    ADULT_LINK_LUT_DL_LFIST = 0x0110
    ADULT_LINK_LUT_DL_LHAND_BOTTLE = 0x0118
    ADULT_LINK_LUT_DL_RHAND = 0x0120
    ADULT_LINK_LUT_DL_RFIST = 0x0128
    ADULT_LINK_LUT_DL_SWORD_SHEATH = 0x0130
    ADULT_LINK_LUT_DL_SWORD_HILT = 0x0138
    ADULT_LINK_LUT_DL_SWORD_BLADE = 0x0140
    ADULT_LINK_LUT_DL_LONGSWORD_HILT = 0x0148
    ADULT_LINK_LUT_DL_LONGSWORD_BLADE = 0x0150
    ADULT_LINK_LUT_DL_LONGSWORD_BROKEN = 0x0158
    ADULT_LINK_LUT_DL_SHIELD_HYLIAN = 0x0160
    ADULT_LINK_LUT_DL_SHIELD_MIRROR = 0x0168
    ADULT_LINK_LUT_DL_HAMMER = 0x0170
    ADULT_LINK_LUT_DL_BOTTLE = 0x0178
    ADULT_LINK_LUT_DL_BOW = 0x0180
    ADULT_LINK_LUT_DL_OCARINA_TIME = 0x0188
    ADULT_LINK_LUT_DL_HOOKSHOT = 0x0190
    ADULT_LINK_LUT_DL_UPGRADE_LFOREARM = 0x0198
    ADULT_LINK_LUT_DL_UPGRADE_LHAND = 0x01A0
    ADULT_LINK_LUT_DL_UPGRADE_LFIST = 0x01A8
    ADULT_LINK_LUT_DL_UPGRADE_RFOREARM = 0x01B0
    ADULT_LINK_LUT_DL_UPGRADE_RHAND = 0x01B8
    ADULT_LINK_LUT_DL_UPGRADE_RFIST = 0x01C0
    ADULT_LINK_LUT_DL_BOOT_LIRON = 0x01C8
    ADULT_LINK_LUT_DL_BOOT_RIRON = 0x01D0
    ADULT_LINK_LUT_DL_BOOT_LHOVER = 0x01D8
    ADULT_LINK_LUT_DL_BOOT_RHOVER = 0x01E0
    ADULT_LINK_LUT_DL_FPS_LFOREARM = 0x01E8
    ADULT_LINK_LUT_DL_FPS_LHAND = 0x01F0
    ADULT_LINK_LUT_DL_FPS_RFOREARM = 0x01F8
    ADULT_LINK_LUT_DL_FPS_RHAND = 0x0200
    ADULT_LINK_LUT_DL_FPS_HOOKSHOT = 0x0208
    ADULT_LINK_LUT_DL_HOOKSHOT_CHAIN = 0x0210
    ADULT_LINK_LUT_DL_HOOKSHOT_HOOK = 0x0218
    ADULT_LINK_LUT_DL_HOOKSHOT_AIM = 0x0220
    ADULT_LINK_LUT_DL_BOW_STRING = 0x0228
    ADULT_LINK_LUT_DL_BLADEBREAK = 0x0230
    ADULT_LINK_LUT_DL_SWORD_SHEATHED = 0x0238
    ADULT_LINK_LUT_DL_SHIELD_HYLIAN_BACK = 0x0258
    ADULT_LINK_LUT_DL_SHIELD_MIRROR_BACK = 0x0268
    ADULT_LINK_LUT_DL_SWORD_SHIELD_HYLIAN = 0x0278
    ADULT_LINK_LUT_DL_SWORD_SHIELD_MIRROR = 0x0288
    ADULT_LINK_LUT_DL_SHEATH0_HYLIAN = 0x0298
    ADULT_LINK_LUT_DL_SHEATH0_MIRROR = 0x02A8
    ADULT_LINK_LUT_DL_LFIST_SWORD = 0x02B8
    ADULT_LINK_LUT_DL_LFIST_LONGSWORD = 0x02D0
    ADULT_LINK_LUT_DL_LFIST_LONGSWORD_BROKEN = 0x02E8
    ADULT_LINK_LUT_DL_LFIST_HAMMER = 0x0300
    ADULT_LINK_LUT_DL_RFIST_SHIELD_HYLIAN = 0x0310
    ADULT_LINK_LUT_DL_RFIST_SHIELD_MIRROR = 0x0320
    ADULT_LINK_LUT_DL_RFIST_BOW = 0x0330
    ADULT_LINK_LUT_DL_RFIST_HOOKSHOT = 0x0340
    ADULT_LINK_LUT_DL_RHAND_OCARINA_TIME = 0x0350
    ADULT_LINK_LUT_DL_FPS_RHAND_BOW = 0x0360
    ADULT_LINK_LUT_DL_FPS_LHAND_HOOKSHOT = 0x0370
    ADULT_HIERARCHY = 0x0380

    CHILD_LINK_LUT_DL_SHIELD_DEKU = 0x00D0
    CHILD_LINK_LUT_DL_WAIST = 0x00D8
    CHILD_LINK_LUT_DL_RTHIGH = 0x00E0
    CHILD_LINK_LUT_DL_RSHIN = 0x00E8
    CHILD_LINK_LUT_DL_RFOOT = 0x00F0
    CHILD_LINK_LUT_DL_LTHIGH = 0x00F8
    CHILD_LINK_LUT_DL_LSHIN = 0x0100
    CHILD_LINK_LUT_DL_LFOOT = 0x0108
    CHILD_LINK_LUT_DL_HEAD = 0x0110
    CHILD_LINK_LUT_DL_HAT = 0x0118
    CHILD_LINK_LUT_DL_COLLAR = 0x0120
    CHILD_LINK_LUT_DL_LSHOULDER = 0x0128
    CHILD_LINK_LUT_DL_LFOREARM = 0x0130
    CHILD_LINK_LUT_DL_RSHOULDER = 0x0138
    CHILD_LINK_LUT_DL_RFOREARM = 0x0140
    CHILD_LINK_LUT_DL_TORSO = 0x0148
    CHILD_LINK_LUT_DL_LHAND = 0x0150
    CHILD_LINK_LUT_DL_LFIST = 0x0158
    CHILD_LINK_LUT_DL_LHAND_BOTTLE = 0x0160
    CHILD_LINK_LUT_DL_RHAND = 0x0168
    CHILD_LINK_LUT_DL_RFIST = 0x0170
    CHILD_LINK_LUT_DL_SWORD_SHEATH = 0x0178
    CHILD_LINK_LUT_DL_SWORD_HILT = 0x0180
    CHILD_LINK_LUT_DL_SWORD_BLADE = 0x0188
    CHILD_LINK_LUT_DL_SLINGSHOT = 0x0190
    CHILD_LINK_LUT_DL_OCARINA_FAIRY = 0x0198
    CHILD_LINK_LUT_DL_OCARINA_TIME = 0x01A0
    CHILD_LINK_LUT_DL_DEKU_STICK = 0x01A8
    CHILD_LINK_LUT_DL_BOOMERANG = 0x01B0
    CHILD_LINK_LUT_DL_SHIELD_HYLIAN_BACK = 0x01B8
    CHILD_LINK_LUT_DL_BOTTLE = 0x01C0
    CHILD_LINK_LUT_DL_MASTER_SWORD = 0x01C8
    CHILD_LINK_LUT_DL_GORON_BRACELET = 0x01D0
    CHILD_LINK_LUT_DL_FPS_RIGHT_ARM = 0x01D8
    CHILD_LINK_LUT_DL_SLINGSHOT_STRING = 0x01E0
    CHILD_LINK_LUT_DL_MASK_BUNNY = 0x01E8
    CHILD_LINK_LUT_DL_MASK_GERUDO = 0x01F0
    CHILD_LINK_LUT_DL_MASK_GORON = 0x01F8
    CHILD_LINK_LUT_DL_MASK_KEATON = 0x0200
    CHILD_LINK_LUT_DL_MASK_SPOOKY = 0x0208
    CHILD_LINK_LUT_DL_MASK_TRUTH = 0x0210
    CHILD_LINK_LUT_DL_MASK_ZORA = 0x0218
    CHILD_LINK_LUT_DL_MASK_SKULL = 0x0220
    CHILD_LINK_DL_SWORD_SHEATHED = 0x0228
    CHILD_LINK_LUT_DL_SWORD_SHEATHED = 0x0248
    CHILD_LINK_DL_SHIELD_DEKU_ODD = 0x0250
    CHILD_LINK_LUT_DL_SHIELD_DEKU_ODD = 0x0260
    CHILD_LINK_DL_SHIELD_DEKU_BACK = 0x0268
    CHILD_LINK_LUT_DL_SHIELD_DEKU_BACK = 0x0278
    CHILD_LINK_DL_SWORD_SHIELD_HYLIAN = 0x0280
    CHILD_LINK_LUT_DL_SWORD_SHIELD_HYLIAN = 0x0290
    CHILD_LINK_DL_SWORD_SHIELD_DEKU = 0x0298
    CHILD_LINK_LUT_DL_SWORD_SHIELD_DEKU = 0x02A8
    CHILD_LINK_DL_SHEATH0_HYLIAN = 0x02B0
    CHILD_LINK_LUT_DL_SHEATH0_HYLIAN = 0x02C0
    CHILD_LINK_DL_SHEATH0_DEKU = 0x02C8
    CHILD_LINK_LUT_DL_SHEATH0_DEKU = 0x02D8
    CHILD_LINK_DL_LFIST_SWORD = 0x02E0
    CHILD_LINK_LUT_DL_LFIST_SWORD = 0x02F8
    CHILD_LINK_DL_LHAND_PEDESTALSWORD = 0x0300
    CHILD_LINK_LUT_DL_LHAND_PEDESTALSWORD = 0x0310
    CHILD_LINK_DL_LFIST_BOOMERANG = 0x0318
    CHILD_LINK_LUT_DL_LFIST_BOOMERANG = 0x0328
    CHILD_LINK_DL_RFIST_SHIELD_DEKU = 0x0330
    CHILD_LINK_LUT_DL_RFIST_SHIELD_DEKU = 0x0340
    CHILD_LINK_DL_RFIST_SLINGSHOT = 0x0348
    CHILD_LINK_LUT_DL_RFIST_SLINGSHOT = 0x0358
    CHILD_LINK_DL_RHAND_OCARINA_FAIRY = 0x0360
    CHILD_LINK_LUT_DL_RHAND_OCARINA_FAIRY = 0x0370
    CHILD_LINK_DL_RHAND_OCARINA_TIME = 0x0378
    CHILD_LINK_LUT_DL_RHAND_OCARINA_TIME = 0x0388
    CHILD_LINK_DL_FPS_RARM_SLINGSHOT = 0x0390
    CHILD_LINK_LUT_DL_FPS_RARM_SLINGSHOT = 0x03A0
    CHILD_HIERARCHY = 0x03A8



# Adult model pieces and their offsets, both in the LUT and in vanilla
AdultPieces: dict[str, tuple[Offsets, int]] = {
    "Sheath": (Offsets.ADULT_LINK_LUT_DL_SWORD_SHEATH, 0x249D8),
    "FPS.Hookshot": (Offsets.ADULT_LINK_LUT_DL_FPS_HOOKSHOT, 0x2A738),
    "Hilt.2": (Offsets.ADULT_LINK_LUT_DL_SWORD_HILT, 0x21F78),  # 0x21F78 + 0xE8, skips blade
    "Hilt.3": (Offsets.ADULT_LINK_LUT_DL_LONGSWORD_HILT, 0x238C8),
    "Blade.2": (Offsets.ADULT_LINK_LUT_DL_SWORD_BLADE, 0x21F78),
    "Hookshot.Spike": (Offsets.ADULT_LINK_LUT_DL_HOOKSHOT_HOOK, 0x2B288),
    "Hookshot": (Offsets.ADULT_LINK_LUT_DL_HOOKSHOT, 0x24D70),
    "Fist.L": (Offsets.ADULT_LINK_LUT_DL_LFIST, 0x21CE8),
    "Fist.R": (Offsets.ADULT_LINK_LUT_DL_RFIST, 0x226E0),
    "FPS.Forearm.L": (Offsets.ADULT_LINK_LUT_DL_FPS_LFOREARM, 0x29FA0),
    "FPS.Forearm.R": (Offsets.ADULT_LINK_LUT_DL_FPS_RFOREARM, 0x29918),
    "Gauntlet.Fist.L": (Offsets.ADULT_LINK_LUT_DL_UPGRADE_LFIST, 0x25438),
    "Gauntlet.Fist.R": (Offsets.ADULT_LINK_LUT_DL_UPGRADE_RFIST, 0x257B8),
    "Gauntlet.Forearm.L": (Offsets.ADULT_LINK_LUT_DL_UPGRADE_LFOREARM, 0x25218),
    "Gauntlet.Forearm.R": (Offsets.ADULT_LINK_LUT_DL_UPGRADE_RFOREARM, 0x25598),
    "Gauntlet.Hand.L": (Offsets.ADULT_LINK_LUT_DL_UPGRADE_LHAND, 0x252D8),
    "Gauntlet.Hand.R": (Offsets.ADULT_LINK_LUT_DL_UPGRADE_RHAND, 0x25658),
    "Bottle.Hand.L": (Offsets.ADULT_LINK_LUT_DL_LHAND_BOTTLE, 0x29600),
    "FPS.Hand.L": (Offsets.ADULT_LINK_LUT_DL_FPS_LHAND, 0x24B58),
    "FPS.Hand.R": (Offsets.ADULT_LINK_LUT_DL_FPS_RHAND, 0x29C20),
    "Bow.String": (Offsets.ADULT_LINK_LUT_DL_BOW_STRING, 0x2B108),
    "Bow": (Offsets.ADULT_LINK_LUT_DL_BOW, 0x22DA8),
    "Blade.3.Break": (Offsets.ADULT_LINK_LUT_DL_BLADEBREAK, 0x2BA38),
    "Blade.3": (Offsets.ADULT_LINK_LUT_DL_LONGSWORD_BLADE, 0x23A28),  # 0x238C8 + 0x160, skips hilt
    "Bottle": (Offsets.ADULT_LINK_LUT_DL_BOTTLE, 0x2AD58),
    "Broken.Blade.3": (Offsets.ADULT_LINK_LUT_DL_LONGSWORD_BROKEN, 0x23EB0),  # 0x23D50 + 0x160, skips hilt
    "Foot.2.L": (Offsets.ADULT_LINK_LUT_DL_BOOT_LIRON, 0x25918),
    "Foot.2.R": (Offsets.ADULT_LINK_LUT_DL_BOOT_RIRON, 0x25A60),
    "Foot.3.L": (Offsets.ADULT_LINK_LUT_DL_BOOT_LHOVER, 0x25BA8),
    "Foot.3.R": (Offsets.ADULT_LINK_LUT_DL_BOOT_RHOVER, 0x25DB0),
    "Hammer": (Offsets.ADULT_LINK_LUT_DL_HAMMER, 0x233E0),
    "Hookshot.Aiming.Reticule": (Offsets.ADULT_LINK_LUT_DL_HOOKSHOT_AIM, 0x2CB48),
    "Hookshot.Chain": (Offsets.ADULT_LINK_LUT_DL_HOOKSHOT_CHAIN, 0x2AFF0),
    "Ocarina.2": (Offsets.ADULT_LINK_LUT_DL_OCARINA_TIME, 0x248D8),  # 0x24698 + 0x240, skips hand
    "Shield.2": (Offsets.ADULT_LINK_LUT_DL_SHIELD_HYLIAN, 0x22970),
    "Shield.3": (Offsets.ADULT_LINK_LUT_DL_SHIELD_MIRROR, 0x241C0),
    "Limb 1": (Offsets.ADULT_LINK_LUT_DL_WAIST, 0x35330),
    "Limb 3": (Offsets.ADULT_LINK_LUT_DL_RTHIGH, 0x35678),
    "Limb 4": (Offsets.ADULT_LINK_LUT_DL_RSHIN, 0x358B0),
    "Limb 5": (Offsets.ADULT_LINK_LUT_DL_RFOOT, 0x358B0),
    "Limb 6": (Offsets.ADULT_LINK_LUT_DL_LTHIGH, 0x35CB8),
    "Limb 7": (Offsets.ADULT_LINK_LUT_DL_LSHIN, 0x35EF0),
    "Limb 8": (Offsets.ADULT_LINK_LUT_DL_LFOOT, 0x361A0),
    "Limb 10": (Offsets.ADULT_LINK_LUT_DL_HEAD, 0x365E8),
    "Limb 11": (Offsets.ADULT_LINK_LUT_DL_HAT, 0x36D30),
    "Limb 12": (Offsets.ADULT_LINK_LUT_DL_COLLAR, 0x362F8),
    "Limb 13": (Offsets.ADULT_LINK_LUT_DL_LSHOULDER, 0x37210),
    "Limb 14": (Offsets.ADULT_LINK_LUT_DL_LFOREARM, 0x373D8),
    "Limb 15": (Offsets.ADULT_LINK_LUT_DL_LHAND, 0x21AA8),
    "Limb 16": (Offsets.ADULT_LINK_LUT_DL_RSHOULDER, 0x36E58),
    "Limb 17": (Offsets.ADULT_LINK_LUT_DL_RFOREARM, 0x37018),
    "Limb 18": (Offsets.ADULT_LINK_LUT_DL_RHAND, 0x22498),
    "Limb 20": (Offsets.ADULT_LINK_LUT_DL_TORSO, 0x363B8),
}


ChildPieces: dict[str, tuple[Offsets, int]] = {
    "Slingshot.String": (Offsets.CHILD_LINK_LUT_DL_SLINGSHOT_STRING, 0x221A8),
    "Sheath": (Offsets.CHILD_LINK_LUT_DL_SWORD_SHEATH, 0x15408),
    "Blade.2": (Offsets.CHILD_LINK_LUT_DL_MASTER_SWORD, 0x15540),  # 0x15540 + 0x158, skips fist
    "Blade.1": (Offsets.CHILD_LINK_LUT_DL_SWORD_BLADE, 0x13F38),  # 0x13F38 + 0x1D8, skips fist and hilt
    "Boomerang": (Offsets.CHILD_LINK_LUT_DL_BOOMERANG, 0x14660),
    "Fist.L": (Offsets.CHILD_LINK_LUT_DL_LFIST, 0x13E18),
    "Fist.R": (Offsets.CHILD_LINK_LUT_DL_RFIST, 0x14320),
    "Hilt.1": (Offsets.CHILD_LINK_LUT_DL_SWORD_HILT, 0x13F38),  # 0x13F38 + 0x110, skips fist
    "Shield.1": (Offsets.CHILD_LINK_LUT_DL_SHIELD_DEKU, 0x14440),
    "Slingshot": (Offsets.CHILD_LINK_LUT_DL_SLINGSHOT, 0x15DF0),  # 0x15DF0 + 0x118, skips fist
    "Ocarina.1": (Offsets.CHILD_LINK_LUT_DL_OCARINA_FAIRY, 0x15BA8),
    "Bottle": (Offsets.CHILD_LINK_LUT_DL_BOTTLE, 0x18478),
    "Ocarina.2": (Offsets.CHILD_LINK_LUT_DL_OCARINA_TIME, 0x15958),  # 0x15958 + 0x160, skips hand
    "Bottle.Hand.L": (Offsets.CHILD_LINK_LUT_DL_LHAND_BOTTLE, 0x18478),  # Just the bottle, couldn't find one with hand and bottle
    "GoronBracelet": (Offsets.CHILD_LINK_LUT_DL_GORON_BRACELET, 0x16118),
    "Mask.Bunny": (Offsets.CHILD_LINK_LUT_DL_MASK_BUNNY, 0x2CA38),
    "Mask.Skull": (Offsets.CHILD_LINK_LUT_DL_MASK_SKULL, 0x2AD40),
    "Mask.Spooky": (Offsets.CHILD_LINK_LUT_DL_MASK_SPOOKY, 0x2AF70),
    "Mask.Gerudo": (Offsets.CHILD_LINK_LUT_DL_MASK_GERUDO, 0x2B788),
    "Mask.Goron": (Offsets.CHILD_LINK_LUT_DL_MASK_GORON, 0x2B350),
    "Mask.Keaton": (Offsets.CHILD_LINK_LUT_DL_MASK_KEATON, 0x2B060),
    "Mask.Truth": (Offsets.CHILD_LINK_LUT_DL_MASK_TRUTH, 0x2B1F0),
    "Mask.Zora": (Offsets.CHILD_LINK_LUT_DL_MASK_ZORA, 0x2B580),
    "FPS.Forearm.R": (Offsets.CHILD_LINK_LUT_DL_FPS_RIGHT_ARM, 0x18048),
    "DekuStick": (Offsets.CHILD_LINK_LUT_DL_DEKU_STICK, 0x6CC0),
    "Shield.2": (Offsets.CHILD_LINK_LUT_DL_SHIELD_HYLIAN_BACK, 0x14B40),  # 0x14B40 + 0xF0, skips sheath
    "Limb 1": (Offsets.CHILD_LINK_LUT_DL_WAIST, 0x202A8),
    "Limb 3": (Offsets.CHILD_LINK_LUT_DL_RTHIGH, 0x204F0),
    "Limb 4": (Offsets.CHILD_LINK_LUT_DL_RSHIN, 0x206E8),
    "Limb 5": (Offsets.CHILD_LINK_LUT_DL_RFOOT, 0x20978),
    "Limb 6": (Offsets.CHILD_LINK_LUT_DL_LTHIGH, 0x20AD8),
    "Limb 7": (Offsets.CHILD_LINK_LUT_DL_LSHIN, 0x20CD0),
    "Limb 8": (Offsets.CHILD_LINK_LUT_DL_LFOOT, 0x20F60),
    "Limb 10": (Offsets.CHILD_LINK_LUT_DL_HEAD, 0x21360),
    "Limb 11": (Offsets.CHILD_LINK_LUT_DL_HAT, 0x219B0),
    "Limb 12": (Offsets.CHILD_LINK_LUT_DL_COLLAR, 0x210C0),
    "Limb 13": (Offsets.CHILD_LINK_LUT_DL_LSHOULDER, 0x21E18),
    "Limb 14": (Offsets.CHILD_LINK_LUT_DL_LFOREARM, 0x21FE8),
    "Limb 15": (Offsets.CHILD_LINK_LUT_DL_LHAND, 0x13CB0),
    "Limb 16": (Offsets.CHILD_LINK_LUT_DL_RSHOULDER, 0x21AE8),
    "Limb 17": (Offsets.CHILD_LINK_LUT_DL_RFOREARM, 0x21CB8),
    "Limb 18": (Offsets.CHILD_LINK_LUT_DL_RHAND, 0x141C0),
    "Limb 20": (Offsets.CHILD_LINK_LUT_DL_TORSO, 0x21130),
}

childSkips: dict[str, list[tuple[int, int]]] = {
    "Blade.1": [(0xA8, 0x1D8)],
    "Blade.2": [(0xA8, 0x158)],
    "Boomerang": [(0x140, 0x240)],
    "Slingshot": [(0xA8, 0x118)],
    #"Hilt.1": [(0xC8, 0x170)],
    "Hilt.1": [(0xA8, 0x110), (0x1D8, 0x280)],
    "Shield.1": [(0x140, 0x218)],
    "Shield.2": [(0xA8, 0xF0)],
    "Ocarina.1": [(0x110, 0x240)],
    "Ocarina.2": [(0xA8, 0x160)]
}

# Note: Some skips which can be implemented by skipping the beginning portion of the model
# rather than specifying those indices here, simply have their offset in the table above
# increased by whatever amount of starting indices would be skipped.
adultSkips: dict[str, list[tuple[int, int]]] = {
    "FPS.Hookshot":  [(0x2F0, 0x618)],
    "Hilt.2": [(0x068, 0x0E8),(0x2D0, 0x518)], # Need to bring in part of the beginning of this item which includes setup DLs
    "Hilt.3": [(0x160, 0x480)],
    "Blade.2": [(0xE8, 0x518)],
    "Hookshot": [(0x250, 0x4A0)],
    "Bow": [(0x158, 0x3B0)],
    "Blade.3": [(0xB8, 0x320)],
    "Broken.Blade.3": [(0xA0, 0x308)],
    "Hammer": [(0x278, 0x4E0)],
    "Shield.2": [(0x158, 0x2B8), (0x3A8, 0x430)],  # Fist is in 2 pieces
    "Shield.3": [(0x1B8, 0x3E8)],
}


# Used to overwrite pointers in the displaylist with new ones
def WriteDLPointer(dl: list[int], index: int, data: int) -> None:
    bytes = data.to_bytes(4, 'big')
    for i in range(4):
        dl[index + i] = bytes[i]

# An extensive function which loads pieces from the vanilla Link model to add to the user-provided zobj
# Based on https://github.com/hylian-modding/ML64-Z64Lib/blob/master/cores/Z64Lib/API/zzoptimize.ts function optimize()
def LoadVanilla(rom: bytearray, missing: list[str], rebase: int, linkstart: int, linksize: int,
                pieces: dict[str, tuple[Offsets, int]], skips: dict[str, list[tuple[int, int]]]) -> tuple[list[int], dict[str, int]]:
    # Get vanilla "zobj" of Link's model
    vanillaData = rom[linkstart: linkstart+linksize]
    segment = 0x06
    vertices = {}
    matrices = {}
    textures = {}
    displayLists = {}
    # For each missing piece, grab data from its vanilla display list
    for item in missing:
        offset = pieces[item][1]
        i = offset
        displayList = []
        # Crawl displaylist bytecode and handle each command
        while i < len(vanillaData):
            # Check if these bytes need to be skipped
            if item in skips.keys():
                skip = False
                for skippedRanges in skips[item]:
                    itemIndex = i - offset
                    # Byte is in a range that must be skipped
                    if skippedRanges[0] <= itemIndex and itemIndex < skippedRanges[1]:
                        skip = True
                if skip:
                    i += 8
                    continue
            op = vanillaData[i]
            seg = vanillaData[i+4]
            lo = int.from_bytes(vanillaData[i+4:i+8], 'big')
            # Source for displaylist bytecode: https://hack64.net/wiki/doku.php?id=f3dex2
            if op == 0xDF: # End of list
                # DF: G_ENDDL
                # Terminates the current displaylist
                # DF 00 00 00 00 00 00 00
                displayList.extend(vanillaData[i:i+8]) # Make sure to write the DF
                break
            # Shouldn't have to deal with DE (branch to new display list)
            elif op == 0x01 and seg == segment: # Vertex data
                # 01: G_VTX
                # Fills the vertex buffer with vertex information
                # 01 0[N N]0 [II] [SS SS SS SS]
                # N: Number of vertices
                # I: Where to start writing vertices inside the vertex buffer (start = II - N*2)
                # S: Segmented address to load vertices from
                # Grab the address from the low byte without teh base offset
                vtxStart = lo & 0x00FFFFFF
                # Grab the length of vertices from the instruction
                # (Number of vertices will be from the 4th and 5th nibble as shown above, but each length 16)
                vtxLen = int.from_bytes(vanillaData[i+1:i+3], 'big')
                if vtxStart not in vertices or len(vertices[vtxStart]) < vtxLen:
                    vertices[vtxStart] = vanillaData[vtxStart:vtxStart+vtxLen]
            elif op == 0xDA and seg == segment: # Push matrix
                # DA: G_MTX
                # Apply transformation matrix
                # DA 38 00 [PP] [AA AA AA AA]
                # P: Parameters for matrix
                # A: Segmented address of vectors of matrix
                # Grab the address from the low byte without the base offset
                mtxStart = lo & 0x00FFFFFF
                if mtxStart not in matrices:
                    matrices[mtxStart] = vanillaData[mtxStart:mtxStart+0x40] # Matrices always 0x40 long
            elif op == 0xFD and seg == segment: # Texture
                # G_SETTIMG
                # Sets the texture image offset
                # FD [fi] 00 00 [bb bb bb bb]
                # [fi] -> fffi i000
                # f: Texture format
                # i: Texture bitsize
                # b: Segmented address of texture
                # Use 3rd nibble to get the texture type
                textureType = (vanillaData[i+1] >> 3) & 0x1F
                # Find the number of texel bits from the type
                numTexelBits = 4 * (2 ** (textureType & 0x3))
                # Get how many bytes there are per texel
                bytesPerTexel = int(numTexelBits / 8)
                # Grab the address from the low byte without the base offset
                texOffset = lo & 0x00FFFFFF
                numTexels = -1
                returnStack = []
                j = i+8
                # The point of this loop is just to find the number of texels
                # so that it may be multiplied by the bytesPerTexel so we know
                # the length of the texture.
                while j < len(vanillaData) and numTexels == -1:
                    opJ = vanillaData[j]
                    segJ = vanillaData[j+4]
                    loJ = int.from_bytes(vanillaData[j+4:j+8], 'big')
                    if opJ == 0xDF:
                        # End of branched texture, or something wrong
                        if len(returnStack) == 0:
                            numTexels = 0
                            break
                        else:
                            j = returnStack.pop()
                    elif opJ == 0xFD:
                        # Another texture command encountered, something wrong
                        numTexels = 0
                        break
                    elif opJ == 0xDE:
                        # Branch to another texture
                        if segJ == segment:
                            if vanillaData[j+1] == 0x0:
                                returnStack.append(j)
                            j = loJ & 0x00FFFFFF
                    elif opJ == 0xF0:
                        # F0: G_LOADTLUT
                        # Loads a number of colors for a pallette
                        # F0 00 00 00 0[t] [cc c]0 00
                        # t: Tile descriptor to load from
                        # c: ((colour count-1) & 0x3FF) << 2
                        # Just grab c from the instruction above
                        # Shift right 12 to get past the first 3 0s, then
                        # 2 more since c is shifted left twice, then add 1
                        # to get the color count of this pallette.
                        numTexels = ((loJ & 0x00FFF000) >> 14) + 1
                        break
                        # Also error if numTexels > 256
                    elif opJ == 0xF3:
                        # F3: G_LOADBLOCK
                        # Determines how much data to load after SETTIMG
                        # F3 [SS S][T TT] 0[I] [XX X][D DD]
                        # S: Upper left corner of texture's S-axis
                        # T: Upper left corner of texture's T-axis
                        # I: Tile descriptor
                        # X: Number of texels to load, minus one
                        # D: dxt (?)
                        # Just grab X from the instruction, shift
                        # right 12 times to get past 0s
                        numTexels = ((loJ & 0x00FFF000) >> 12) + 1
                        break
                    j += 8
                dataLen = bytesPerTexel * numTexels
                if texOffset not in textures or len(textures[texOffset]) < dataLen:
                    textures[texOffset] = vanillaData[texOffset:texOffset+dataLen]
            displayList.extend(vanillaData[i:i+8])
            i += 8
        displayLists[item] = (displayList, offset)
    # Create vanilla zobj of the pieces from data collected during crawl
    vanillaZobj = []
    # Add textures, vertices, and matrices to the beginning of the zobj
    # Textures
    oldTex2New = {}
    for (offset, texture) in textures.items():
        newOffset = len(vanillaZobj)
        oldTex2New[offset] = newOffset
        vanillaZobj.extend(texture)
    # Vertices
    oldVer2New = {}
    for (offset, vertex) in vertices.items():
        newOffset = len(vanillaZobj)
        oldVer2New[offset] = newOffset
        vanillaZobj.extend(vertex)
    # Matrices
    oldMtx2New = {}
    for (offset, matrix) in matrices.items():
        newOffset = len(vanillaZobj)
        oldMtx2New[offset] = newOffset
        vanillaZobj.extend(matrix)
    # Now add display lists which will reference the data from the beginning of the zobj
    # Display lists
    oldDL2New = {}
    pieceDLs = {}
    for piece in displayLists:
        data = displayLists[piece]
        dl = data[0]
        offset = data[1]
        oldDL2New[offset] = len(vanillaZobj)
        pieceDLs[piece] = len(vanillaZobj)
        for i in range (0, len(dl), 8):
            op = dl[i]
            seg = dl[i+4]
            lo = int.from_bytes(dl[i+4:i+8], 'big')
            if seg == segment:
                # If this instruction points to some data, it must be repointed
                if op == 0x01:
                    vertEntry = oldVer2New[lo & 0x00FFFFFF]
                    WriteDLPointer(dl, i + 4, BASE_OFFSET + vertEntry + rebase)
                elif op == 0xDA:
                    mtxEntry = oldMtx2New[lo & 0x00FFFFFF]
                    WriteDLPointer(dl, i + 4, BASE_OFFSET + mtxEntry + rebase)
                elif op == 0xFD:
                    texEntry = oldTex2New[lo & 0x00FFFFFF]
                    WriteDLPointer(dl, i + 4, BASE_OFFSET + texEntry + rebase)
                elif op == 0xDE:
                    dlEntry = oldDL2New[lo & 0x00FFFFFF]
                    WriteDLPointer(dl, i + 4, BASE_OFFSET + dlEntry + rebase)
        vanillaZobj.extend(dl)
        # Pad to the nearest multiple of 16
        while len(vanillaZobj) % 0x10 != 0:
            vanillaZobj.append(0x00)
    # Now find the relation of items to new offsets
    DLOffsets = {}
    for item in missing:
        DLOffsets[item] = pieceDLs[item]
    return vanillaZobj, DLOffsets

def LoadVanillaPiecesToZOBJ(zobj: bytearray, rom: bytearray, age: int):
    
    lut_start = zobj.find("MODLOADER64".encode())
    lut = LUT(0x06000000 | lut_start)
    
    # age 0 = adult, 1 = child
    linkstart = ADULT_START
    linksize = ADULT_SIZE
    pieces = AdultPieces
    skips = adultSkips
    agestr = "adult" # Just used for error messages
    if age == 1:
        linkstart = CHILD_START
        linksize = CHILD_SIZE
        pieces = ChildPieces
        skips = childSkips
        agestr = "child"
    
    missing = []
    for piece in pieces:
        lut_offset, _ = pieces[piece]
        lut_offset = lut.offset(lut_offset) & 0x00FFFFFF
        lut_entry = int.from_bytes(zobj[lut_offset:lut_offset+4], 'big')
        if lut_entry == 0xFFFFFFFF:
            print(f"Missing piece {piece}")
            missing.append(piece)

    vanilla_dl_base = len(zobj)
    (vanillaZobj, DLOffsets) = LoadVanilla(rom, missing, len(zobj), linkstart, linksize, pieces, skips)
    # Add the parts to the end of the zobj and update the LUT
    zobj.extend(vanillaZobj)
    for missingPieceOffset in DLOffsets:
        lut_offset, _ = pieces[missingPieceOffset]
        lut_offset = lut.offset(lut_offset) & 0x00FFFFFF
        lut_entry = vanilla_dl_base + DLOffsets[missingPieceOffset]
        zobj[lut_offset:lut_offset+4] = b"\xDE\x01\x00\x00"
        zobj[lut_offset+4:lut_offset+8] = (lut_entry | BASE_OFFSET).to_bytes(4, 'big') 
    
    # Write skeleton to the end of the LUT because MODLOADER does that for some reason
    # Find the one compiled into the file

    print(hex(lut_start))