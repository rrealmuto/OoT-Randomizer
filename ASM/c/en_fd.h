#ifndef Z_EN_FD_H
#define Z_EN_FD_H

#include "z64.h"
#include "animation.h"
#include "z64collision_check.h"

struct EnFd;

typedef void (*EnFdActionFunc)(struct EnFd* this, struct PlayState* play);

typedef enum FDEffectType {
    FD_EFFECT_NONE,
    FD_EFFECT_FLAME,
    FD_EFFECT_DOT
} FDEffectType;

#define EN_FD_EFFECT_COUNT 200

typedef struct EnFdEffect {
    /* 0x0000 */ uint8_t type;
    /* 0x0001 */ uint8_t timer;
    /* 0x0002 */ uint8_t initialTimer;
    /* 0x0004 */ float scale;
    /* 0x0008 */ float scaleStep;
    /* 0x000C */ colorRGBA8_t color;
    /* 0x0010 */ char unk_10[4];
    /* 0x0014 */ z64_xyzf_t pos;
    /* 0x0020 */ z64_xyzf_t velocity;
    /* 0x002C */ z64_xyzf_t accel;
} EnFdEffect; // size = 0x38

typedef struct EnFd {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ SkelAnime skelAnime;
    /* 0x0190 */ EnFdActionFunc actionFunc;
    /* 0x0194 */ ColliderJntSph collider;
    /* 0x01B4 */ ColliderJntSphElement colliderElements[12];
    /* 0x04B4 */ uint8_t coreActive;
    /* 0x04B6 */ int16_t initYawToInitPos;
    /* 0x04B8 */ int16_t curYawToInitPos;
    /* 0x04BA */ int16_t runDir;
    /* 0x04BC */ int16_t firstUpdateFlag;
    /* 0x04BE */ int16_t spinTimer;
    /* 0x04C0 */ int16_t circlesToComplete;
    /* 0x04C2 */ int16_t invincibilityTimer;
    /* 0x04C4 */ int16_t attackTimer;
    /* 0x04C8 */ float runRadius;
    /* 0x04CC */ float fadeAlpha;
    /* 0x04D0 */ z64_xyzf_t corePos;
    /* 0x04DC */ z64_xyz_t jointTable[27];
    /* 0x057E */ z64_xyz_t morphTable[27];
    /* 0x0620 */ EnFdEffect effects[EN_FD_EFFECT_COUNT];
} EnFd; // size = 0x31E0

#endif
