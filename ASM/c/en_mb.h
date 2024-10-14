#ifndef __EN_MB_H__
#define __EN_MB_H__

#include "z64.h"

struct EnMb;

typedef void (*EnMbActionFunc)(struct EnMb*, z64_game_t*);

typedef enum EnMbState {
    /*  0 */ ENMB_STATE_SPEAR_SPEARPATH_DAMAGED,
    /*  1 */ ENMB_STATE_CLUB_DEAD,
    /*  2 */ ENMB_STATE_CLUB_KNEELING_DAMAGED,
    /*  3 */ ENMB_STATE_CLUB_KNEELING,
    /*  5 */ ENMB_STATE_STUNNED = 5,
    /*  6 */ ENMB_STATE_IDLE,
    /*  9 */ ENMB_STATE_WALK = 9,
    /* 10 */ ENMB_STATE_ATTACK,
    /* 11 */ ENMB_STATE_ATTACK_END
} EnMbState;

typedef struct EnMb {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ z64_xyz_t bodyPartsPos[10];
    /* 0x0188 */ uint8_t damageEffect;
    /* 0x018C */ SkelAnime skelAnime;
    /* 0x01D0 */ z64_xyz_t jointTable[28];
    /* 0x0278 */ z64_xyz_t morphTable[28];
    /* 0x0320 */ EnMbState state;
    /* 0x0324 */ EnMbActionFunc actionFunc;
    /* 0x0328 */ int16_t iceEffectTimer;
    /* 0x032A */ int16_t timer1;
    /* 0x032C */ int16_t timer2;
    /* 0x032E */ int16_t timer3;
    /* 0x0330 */ int16_t yawToWaypoint;
    /* 0x0332 */ int16_t unk_332;
    /* 0x0334 */ int16_t attack;
    /* 0x0338 */ z64_xyzf_t effSpawnPos;
    /* 0x0344 */ z64_xyzf_t waypointPos;
    /* 0x0350 */ char unk_34A[0xC];
    /* 0x035C */ int8_t waypoint;
    /* 0x035D */ int8_t path;
    /* 0x035E */ int8_t direction;
    /* 0x0360 */ float maxHomeDist;
    /* 0x0364 */ float playerDetectionRange;
    /* 0x0368 */ ColliderCylinder bodyCollider;
    /* 0x03B4 */ ColliderQuad attackCollider; // for attacking the player
    /* 0x0434 */ ColliderTris frontShielding; // Moblins don't have shields, but this acts as one
    /* 0x0454 */ ColliderTrisElement frontShieldingTris[2];
} EnMb; // size = 0x050C

#endif
