//
// Created by vcunningham on 10/12/23.
//

#ifndef KALEIDO_H
#define KALEIDO_H

#define AGE_REQ_ADULT 0
#define AGE_REQ_CHILD 1
#define AGE_REQ_NONE 9

#define CHECK_AGE_REQ_SLOT(slot) \
    ((gSlotAgeReqs[slot] == AGE_REQ_NONE) || gSlotAgeReqs[slot] == ((void)0, z64_file.link_age))

extern uint8_t OVL_gSlotAgeReqs[24];
extern int16_t OVL_sEquipState;
extern int16_t OVL_sEquipAnimTimer;
extern int16_t OVL_sEquipMoveTimer;

typedef enum {
    /* 0x00 */ PAUSE_ITEM,
    /* 0x01 */ PAUSE_MAP,
    /* 0x02 */ PAUSE_QUEST,
    /* 0x03 */ PAUSE_EQUIP,
    /* 0x04 */ PAUSE_WORLD_MAP
} pause_menu_page;

#endif //KALEIDO_H
