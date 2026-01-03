#ifndef ENTRANCES_H
#define ENTRANCES_H

#include <stdint.h>

#define ENTR_MAX 0x614

typedef struct EntranceInfo {
    /* 0x00 */ int8_t  sceneId;
    /* 0x01 */ int8_t  spawn;
    /* 0x02 */ uint16_t field;
} EntranceInfo; // size = 0x4

extern EntranceInfo gEntranceTable[ENTR_MAX];

#endif