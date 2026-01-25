#ifndef Z_EN_RD_H
#define Z_EN_RD_H

#include "z64.h"
#include "actor.h"
#include "animation.h"

struct EnRd;

typedef void (*EnRdActionFunc)(struct EnRd*, struct PlayState*);

#define REDEAD_GET_RDFLAGS(thisx) PARAMS_GET_S((thisx)->params, 8, 8)

typedef enum RedeadGibdoLimb {
    /*  0 */ REDEAD_GIBDO_LIMB_NONE,
    /*  1 */ REDEAD_GIBDO_LIMB_ROOT,
    /*  2 */ REDEAD_GIBDO_LIMB_LEFT_LEG_ROOT,
    /*  3 */ REDEAD_GIBDO_LIMB_LEFT_THIGH,
    /*  4 */ REDEAD_GIBDO_LIMB_LEFT_SHIN,
    /*  5 */ REDEAD_GIBDO_LIMB_LEFT_FOOT_ROOT,
    /*  6 */ REDEAD_GIBDO_LIMB_LEFT_FOOT,
    /*  7 */ REDEAD_GIBDO_LIMB_RIGHT_LEG_ROOT,
    /*  8 */ REDEAD_GIBDO_LIMB_RIGHT_THIGH,
    /*  9 */ REDEAD_GIBDO_LIMB_RIGHT_SHIN,
    /* 10 */ REDEAD_GIBDO_LIMB_RIGHT_FOOT_ROOT,
    /* 11 */ REDEAD_GIBDO_LIMB_RIGHT_FOOT,
    /* 12 */ REDEAD_GIBDO_LIMB_UPPER_BODY_ROOT,
    /* 13 */ REDEAD_GIBDO_LIMB_TORSO_ROOT,
    /* 14 */ REDEAD_GIBDO_LIMB_TORSO,
    /* 15 */ REDEAD_GIBDO_LIMB_LEFT_ARM_ROOT,
    /* 16 */ REDEAD_GIBDO_LIMB_LEFT_UPPER_ARM,
    /* 17 */ REDEAD_GIBDO_LIMB_LEFT_FOREARM,
    /* 18 */ REDEAD_GIBDO_LIMB_LEFT_HAND,
    /* 19 */ REDEAD_GIBDO_LIMB_RIGHT_ARM_ROOT,
    /* 20 */ REDEAD_GIBDO_LIMB_RIGHT_UPPER_ARM,
    /* 21 */ REDEAD_GIBDO_LIMB_RIGHT_FOREARM,
    /* 22 */ REDEAD_GIBDO_LIMB_RIGHT_HAND,
    /* 23 */ REDEAD_GIBDO_LIMB_HEAD_ROOT,
    /* 24 */ REDEAD_GIBDO_LIMB_HEAD,
    /* 25 */ REDEAD_GIBDO_LIMB_PELVIS,
    /* 26 */ REDEAD_GIBDO_LIMB_MAX
} RedeadGibdoLimb;

typedef enum EnRdType {
    /* -3 */ REDEAD_TYPE_GIBDO_RISING_OUT_OF_COFFIN = -3,
    /* -2 */ REDEAD_TYPE_GIBDO,
    /* -1 */ REDEAD_TYPE_DOES_NOT_MOURN,
    /*  0 */ REDEAD_TYPE_DOES_NOT_MOURN_IF_WALKING,
    /*  1 */ REDEAD_TYPE_REGULAR,
    /*  2 */ REDEAD_TYPE_CRYING,
    /*  3 */ REDEAD_TYPE_INVISIBLE
} EnRdType;

typedef struct EnRd {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ z64_xyz_t firePos[10];
    /* 0x0188 */ SkelAnime skelAnime;
    /* 0x01CC */ z64_xyz_t jointTable[REDEAD_GIBDO_LIMB_MAX];
    /* 0x0268 */ z64_xyz_t morphTable[REDEAD_GIBDO_LIMB_MAX];
    /* 0x0304 */ uint8_t grabState;
    /* 0x0305 */ uint8_t isMourning;
    /* 0x0306 */ uint8_t playerStunWaitTimer; // Cannot stun the player if this is non-zero
    /* 0x0307 */ uint8_t grabWaitTimer; // Cannot grab the player if this is non-zero
    /* 0x0308 */ EnRdActionFunc actionFunc;
    /* 0x030C */ int16_t timer;
    /* 0x030E */ int16_t headYRotation;
    /* 0x0310 */ int16_t upperBodyYRotation;
    /* 0x0312 */ int16_t rdFlags;
    /* 0x0314 */ int16_t alpha;
    /* 0x0316 */ int16_t sunsSongStunTimer;
    /* 0x0318 */ uint8_t stunnedBySunsSong;
    /* 0x0319 */ uint8_t grabDamageTimer;
    /* 0x031A */ uint8_t fireTimer;
    /* 0x031B */ uint8_t action;
    /* 0x031C */ uint8_t damageReaction;
    /* 0x031D */ uint8_t unk_31D; // related to player->unk_845
    /* 0x0320 */ ColliderCylinder collider;
} EnRd; // size = 0x036C

#endif
