#ifndef FAIRY_H
#define FAIRY_H

#include "z64.h"
#include "animation.h"
#include "z64_math.h"
#include "color.h"
#include "models.h"

struct EnElf;

typedef void (*EnElfActionFunc)(struct EnElf*, z64_game_t*);
typedef void (*EnElfUnkFunc)(struct EnElf*, z64_game_t*);


#define FAIRY_FLAG_TIMED (1 << 8)
#define FAIRY_FLAG_BIG (1 << 9)
#define FAIRY_FLAG_OVERRIDE_COLLECTED (1 << 15)

typedef struct EnElf {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ SkelAnime skelAnime;
    /* 0x0190 */ z64_xyz_t jointTable[15];
    /* 0x01EA */ z64_xyz_t morphTable[15];
    /* 0x0244 */ colorRGBAf_t innerColor;
    /* 0x0254 */ colorRGBAf_t outerColor;
    /* 0x0264 */ uint8_t lightInfoGlow[0x10];  //LightInfo lightInfoGlow;
    /* 0x0274 */ uint8_t lightNodeGlow[0x4];  //LightNode* lightNodeGlow;
    /* 0x0278 */ uint8_t lightInfoNoGlow[0x10];  //LightInfo lightInfoNoGlow;
    /* 0x0288 */ uint8_t lightNodeNoGlow[0x4];  //LightNode* lightNodeNoGlow;
    /* 0x028C */ z64_xyzf_t unk_28C;
    /* 0x0298 */ struct ElfMsg* elfMsg;
    /* 0x029C */ float unk_29C;
    /* 0x02A0 */ float unk_2A0;
    /* 0x02A4 */ float unk_2A4;
    /* 0x02A8 */ int16_t unk_2A8;
    /* 0x02AA */ int16_t unk_2AA;
    /* 0x02AC */ int16_t unk_2AC;
    /* 0x02AE */ int16_t unk_2AE;
    /* 0x02B0 */ int16_t unk_2B0;
    /* 0x02B4 */ float unk_2B4;
    /* 0x02B8 */ float unk_2B8;
    /* 0x02BC */ int16_t unk_2BC;
    /* 0x02BE */ uint16_t timer;
    /* 0x02C0 */ int16_t unk_2C0;
    /* 0x02C2 */ int16_t disappearTimer;
    /* 0x02C4 */ uint16_t fairyFlags;
    /* 0x02C6 */ uint8_t unk_2C6;
    /* 0x02C7 */ uint8_t unk_2C7;
    /* 0x02C8 */ EnElfUnkFunc func_2C8;
    /* 0x02CC */ EnElfActionFunc actionFunc;
    /* 0x02D0 */ override_t override;
    /* 0x02E0 */ model_t model;

} EnElf; // size = 0x02D0



struct ObjMure;
typedef void (*ObjMureActionFunc)(struct ObjMure*, z64_game_t*);
#define OBJMURE_MAX_SPAWNS 15
typedef struct ObjMure {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ ObjMureActionFunc actionFunc;
    /* 0x0150 */ int16_t chNum;
    /* 0x0152 */ int16_t ptn;
    /* 0x0154 */ int16_t svNum;
    /* 0x0156 */ int16_t type;
    /* 0x0158 */ z64_actor_t* children[OBJMURE_MAX_SPAWNS];
    /* 0x0194 */ uint8_t childrenStates[OBJMURE_MAX_SPAWNS];
    /* 0x01A4 */ int16_t unk_1A4;
    /* 0x01A6 */ int16_t unk_1A6;
    /* 0x01A8 */ int16_t unk_1A8;
} ObjMure; // size = 0x01AC

void EnElf_WaitForMessageBox(EnElf* this, z64_game_t* play);

#endif