#ifndef EN_GS_H
#define EN_GS_H

#include "z64.h"
#include "stdint.h"

typedef enum {
    /* 0x00 */ FAIRY_NAVI,
    /* 0x01 */ FAIRY_REVIVE_BOTTLE,
    /* 0x02 */ FAIRY_HEAL_TIMED,
    /* 0x03 */ FAIRY_KOKIRI,
    /* 0x04 */ FAIRY_SPAWNER,
    /* 0x05 */ FAIRY_REVIVE_DEATH,
    /* 0x06 */ FAIRY_HEAL,
    /* 0x07 */ FAIRY_HEAL_BIG
} FairyType;

struct EnGs;

typedef void (*EnGsActionFunc)(struct EnGs*, z64_game_t*);

typedef struct EnGs {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ uint8_t collider[0x4c];
    /* 0x0198 */ EnGsActionFunc actionFunc;
    /* 0x019C */ uint8_t unk_19C;
    /* 0x019D */ uint8_t unk_19D;
    /* 0x019E */ uint8_t unk_19E;
    /* 0x019F */ uint8_t unk_19F;
    /* 0x01A0 */ z64_xyz_t unk_1A0[3];
    /* 0x01B4 */ z64_xyzf_t unk_1B4[2];
    /* 0x01CC */ uint8_t unk_1CC[0xC];
    /* 0x01D8 */ z64_xyzf_t unk_1D8;
    /* 0x01E4 */ colorRGBA8_t flashColor;
    /* 0x01E8 */ float unk_1E8;
    /* 0x01EC */ float unk_1EC;
    /* 0x01F0 */ float unk_1F0;
    /* 0x01F4 */ float unk_1F4;
    /* 0x01F8 */ float unk_1F8;
    /* 0x01FC */ float unk_1FC;
    /* 0x0200 */ uint16_t unk_200;
    /* 0x0202 */ char unk_202[0x6];
} EnGs; // size = 0x0208

#endif
