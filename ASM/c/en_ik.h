#ifndef __EN_IK_H__
#define __EN_IK_H__
#include "z64.h"

struct EnIk;

typedef void (*EnIkActionFunc)(struct EnIk*, z64_game_t*);

typedef enum EnIkType {
    /* 0 */ IK_TYPE_NABOORU,
    /* 1 */ IK_TYPE_SILVER,
    /* 2 */ IK_TYPE_BLACK,
    /* 3 */ IK_TYPE_WHITE
} EnIkType;

typedef enum IronKnuckleLimb {
    /* 0x00 */ IRON_KNUCKLE_LIMB_NONE,
    /* 0x01 */ IRON_KNUCKLE_LIMB_ROOT,
    /* 0x02 */ IRON_KNUCKLE_LIMB_LOWER_BODY,
    /* 0x03 */ IRON_KNUCKLE_LIMB_RIGHT_THIGH,
    /* 0x04 */ IRON_KNUCKLE_LIMB_RIGHT_SHIN,
    /* 0x05 */ IRON_KNUCKLE_LIMB_RIGHT_FOOT,
    /* 0x06 */ IRON_KNUCKLE_LIMB_LEFT_THIGH,
    /* 0x07 */ IRON_KNUCKLE_LIMB_LEFT_SHIN,
    /* 0x08 */ IRON_KNUCKLE_LIMB_LEFT_FOOT,
    /* 0x09 */ IRON_KNUCKLE_LIMB_UPPER_BODY_ROOT,
    /* 0x0A */ IRON_KNUCKLE_LIMB_UPPER_BODY,
    /* 0x0B */ IRON_KNUCKLE_LIMB_HEAD_ROOT,
    /* 0x0C */ IRON_KNUCKLE_LIMB_HELMET_ARMOR,
    /* 0x0D */ IRON_KNUCKLE_LIMB_HEAD,
    /* 0x0E */ IRON_KNUCKLE_LIMB_RIGHT_UPPER_ARM,
    /* 0x0F */ IRON_KNUCKLE_LIMB_RIGHT_FOREARM,
    /* 0x10 */ IRON_KNUCKLE_LIMB_RIGHT_HAND_ROOT,
    /* 0x11 */ IRON_KNUCKLE_LIMB_AXE,
    /* 0x12 */ IRON_KNUCKLE_LIMB_RIGHT_HAND,
    /* 0x13 */ IRON_KNUCKLE_LIMB_LEFT_UPPER_ARM,
    /* 0x14 */ IRON_KNUCKLE_LIMB_LEFT_FOREARM,
    /* 0x15 */ IRON_KNUCKLE_LIMB_LEFT_HAND,
    /* 0x16 */ IRON_KNUCKLE_LIMB_UPPER_LEFT_PAULDRON,
    /* 0x17 */ IRON_KNUCKLE_LIMB_LOWER_LEFT_PAULDRON,
    /* 0x18 */ IRON_KNUCKLE_LIMB_UPPER_RIGHT_PAULDRON,
    /* 0x19 */ IRON_KNUCKLE_LIMB_LOWER_RIGHT_PAULDRON,
    /* 0x1A */ IRON_KNUCKLE_LIMB_CHEST_ARMOR_FRONT,
    /* 0x1B */ IRON_KNUCKLE_LIMB_CHEST_ARMOR_BACK,
    /* 0x1C */ IRON_KNUCKLE_LIMB_TORSO,
    /* 0x1D */ IRON_KNUCKLE_LIMB_WAIST,
    /* 0x1E */ IRON_KNUCKLE_LIMB_MAX
} IronKnuckleLimb;

typedef struct EnIk {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ SkelAnime skelAnime;
    /* 0x0190 */ z64_xyz_t jointTable[IRON_KNUCKLE_LIMB_MAX];
    /* 0x0244 */ z64_xyz_t morphTable[IRON_KNUCKLE_LIMB_MAX];
    /* 0x02F8 */ uint8_t unk_2F8;
    /* 0x02F9 */ uint8_t animationTimer;
    /* 0x02FA */ uint8_t drawArmorFlag;
    /* 0x02FB */ uint8_t armorStatusFlag;
    /* 0x02FC */ uint8_t isBreakingProp;
    /* 0x02FD */ uint8_t damageEffect;
    /* 0x02FE */ int8_t unk_2FE;
    /* 0x02FF */ int8_t unk_2FF;
    /* 0x0300 */ int16_t unk_300;
    /* 0x0302 */ int16_t switchFlag;
    /* 0x0304 */ EnIkActionFunc actionFunc;
    /* 0x0308 */ BodyBreak bodyBreak;
    /* 0x0320 */ ColliderCylinder bodyCollider;
    /* 0x036C */ ColliderQuad axeCollider;
    /* 0x03EC */ ColliderTris shieldCollider;
    /* 0x040C */ ColliderTrisElement shieldColliderItems[2];
    /* 0x04C4 */ int32_t blureIdx;
    /* 0x04C8 */ int32_t csAction;
    /* 0x04CC */ int32_t csDrawMode;
    /* 0x04D0 */ uint32_t cueId;
    /* 0x04D4 */ int32_t isAxeSummoned;
    /* 0x04D8 */ char unk_4D8[0x04];
    /* 0x04DC */ uint8_t spawnMode;
} EnIk; // size = 0x04DC

#endif
