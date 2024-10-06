#ifndef __EN_ENCOUNT1_H__
#define __EN_ENCOUNT1_H__

#include "z64.h"

#define SPAWNER_PARAMS(type, number, total) ((type << 0xB) | (number << 0x6) | total)

struct EnEncount1;

typedef void (*EnEncount1UpdateFunc)(struct EnEncount1*, z64_game_t*);

typedef enum EnEncount1type {
    /* 0 */ SPAWNER_LEEVER,
    /* 1 */ SPAWNER_TEKTITE,
    /* 2 */ SPAWNER_STALCHILDREN,
    /* 3 */ SPAWNER_WOLFOS
} EnEncount1type;

typedef struct EnEncount1 {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x014C */ EnEncount1UpdateFunc updateFunc;
    /* 0x0150 */ int16_t maxCurSpawns;
    /* 0x0152 */ int16_t curNumSpawn;
    /* 0x0154 */ int16_t spawnType;
    /* 0x0156 */ int16_t maxTotalSpawns;
    /* 0x0158 */ int16_t totalNumSpawn;
    /* 0x015A */ int16_t outOfRangeTimer;
    /* 0x015C */ int16_t fieldSpawnTimer;
    /* 0x015E */ int16_t killCount;
    /* 0x0160 */ int16_t numLeeverSpawns;
    /* 0x0162 */ int16_t leeverIndex;
    /* 0x0164 */ int16_t timer;
    /* 0x0166 */ uint8_t reduceLeevers;
    /* 0x0168 */ float spawnRange;
    /* 0x016C */ z64_actor_t* bigLeever;
} EnEncount1; // size = 0x0170

#endif
