#ifndef ITEM_UPGRADES_H
#define ITEM_UPGRADES_H

#include "get_items.h"
#include "z64.h"

extern uint8_t CUSTOM_KEY_MODELS;

uint16_t no_upgrade(z64_file_t* save, override_t override);
uint16_t hookshot_upgrade(z64_file_t* save, override_t override);
uint16_t strength_upgrade(z64_file_t* save, override_t override);
uint16_t bomb_bag_upgrade(z64_file_t* save, override_t override);
uint16_t bow_upgrade(z64_file_t* save, override_t override);
uint16_t slingshot_upgrade(z64_file_t* save, override_t override);
uint16_t wallet_upgrade(z64_file_t* save, override_t override);
uint16_t scale_upgrade(z64_file_t* save, override_t override);
uint16_t nut_upgrade(z64_file_t* save, override_t override);
uint16_t stick_upgrade(z64_file_t* save, override_t override);
uint16_t magic_upgrade(z64_file_t* save, override_t override);
uint16_t bombchu_upgrade(z64_file_t* save, override_t override);
uint16_t ocarina_upgrade(z64_file_t* save, override_t override);
uint16_t arrows_to_rupee(z64_file_t* save, override_t override);
uint16_t bombs_to_rupee(z64_file_t* save, override_t override);
uint16_t seeds_to_rupee(z64_file_t* save, override_t override);
uint16_t letter_to_bottle(z64_file_t* save, override_t override);
uint16_t health_upgrade_cap(z64_file_t* save, override_t override);
uint16_t bombchus_to_bag(z64_file_t* save, override_t override);
uint16_t upgrade_key_model(z64_file_t* save, override_t override);

// The layout of this struct is part of the definition of the co-op context.
// If you change it, bump the co-op context version in coop_state.asm and update Notes/coop-ctx.md
typedef struct {
    uint16_t _pad : 9;  // reserved for future use
    bool chu_bag : 1;  // added in version 6 of the co-op context
    uint8_t ocarina : 2;  // 0 = no ocarina, 1 = fairy ocarina, 2 = ocarina of time
    uint8_t magic : 2;  // 0 = no magic, 1 = single magic, 2 = double magic
    uint8_t sticks : 2;  // 0 = no sticks, 1 = 10, 2 = 20, 3 = 30
    uint8_t nuts : 2;  // 0 = no nuts, 1 = 20, 2 = 30, 3 = 40
    uint8_t scale : 2;  // 0 = no scale, 1 = silver scale, 2 = gold scale
    uint8_t wallet : 2;  // 0 = 99, 1 = 200, 2 = 500, 3 = 999
    uint8_t slingshot : 2;  // 0 = no slingshot, 1 = 30, 2 = 40, 3 = 50
    uint8_t bow : 2;  // 0 = no bow, 1 = 30, 2 = 40, 3 = 50
    uint8_t bomb_bag : 2;  // 0 = no bomb bag, 1 = 20, 2 = 30, 3 = 40
    uint8_t strength : 2;  // 0 = no strength, 1 = Goron bracelet, 2 = silver gauntlets, 3 = gold gauntlets
    uint8_t hookshot : 2;  // 0 = no hookshot, 1 = hookshot, 2 = longshot
} mw_progressive_items_state_t;

#endif
