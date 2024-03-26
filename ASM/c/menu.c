#include "z64.h"
#include "util.h"
#include <stdbool.h>

#define gSlotAgeReqsAddr 0x80829d74
#define gItemAgeReqsAddr 0x80829d9c

#define AGE_REQ_ADULT LINK_AGE_ADULT
#define AGE_REQ_CHILD LINK_AGE_CHILD
#define AGE_REQ_NONE 9

#define CHECK_AGE_REQ_SLOT(slot) ((gSlotAgeReqs[slot] == AGE_REQ_NONE) || gSlotAgeReqs[slot] == ((void)0, z64_file.link_age))
#define CHECK_AGE_REQ_ITEM(item) ((gItemAgeReqs[item] == AGE_REQ_NONE) || (gItemAgeReqs[item] == ((void)0, z64_file.link_age)))

_Bool c_equipment_menu_slot_filled() {
    if (z64_game.pause_ctxt.equipment_x == 0) {
        if (z64_game.pause_ctxt.equipment_y == 0) {
            if (z64_file.link_age == 0 && (z64_file.bullet_bag || z64_file.quiver)) return 1;
            else return z64_file.bullet_bag;
        }
        return (z64_file.equipment_items >> (3 * z64_game.pause_ctxt.equipment_y)) & 0x07;
    }
    if (z64_game.pause_ctxt.equipment_y == 0) {
        if (z64_game.pause_ctxt.equipment_x != 3)  goto retnormal;
        return z64_file.broken_giants_knife || z64_file.giants_knife;
    }
retnormal:
    return (z64_file.equipment >> ((4 * z64_game.pause_ctxt.equipment_y) + (z64_game.pause_ctxt.equipment_x - 1))) & 0x01;
}

bool check_slot_age_requirements(uint16_t cursorSlot) {
    uint8_t* gSlotAgeReqs = resolve_kaleido_ovl_addr(gSlotAgeReqsAddr);

    // Get the item in the slot
    int8_t item = z64_file.items[cursorSlot];
    if(item == ITEM_MASK_BUNNY)
        return true;

    return CHECK_AGE_REQ_SLOT(cursorSlot);
}

bool check_item_age_requirements(uint16_t item) {
    uint8_t* gItemAgeReqs = resolve_kaleido_ovl_addr(gItemAgeReqsAddr);
    uint8_t* gSlotAgeReqs = resolve_kaleido_ovl_addr(gSlotAgeReqsAddr);
    
    if(item == ITEM_MASK_BUNNY)
        return true;

    return CHECK_AGE_REQ_ITEM(item);
}