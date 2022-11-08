#ifndef GET_ITEMS_H
#define GET_ITEMS_H

#include <stdbool.h>
#include "z64.h"

void item_overrides_init();
void handle_pending_items();
void push_delayed_item(uint8_t flag);
void pop_pending_item();
enum override_type {
    OVR_BASE_ITEM = 0,
    OVR_CHEST = 1,
    OVR_COLLECTABLE = 2,
    OVR_SKULL = 3,
    OVR_GROTTO_SCRUB = 4,
    OVR_DELAYED = 5,
    OVR_NEWFLAGCOLLECTIBLE = 6,
};

typedef union {
    uint32_t all;
    struct {
        uint8_t scene;
        uint8_t type;
        uint16_t flag;
    };
} override_key_t;

typedef union {
    uint32_t all;
    struct {
        uint16_t item_id;
        uint8_t  player;
        uint8_t  looks_like_item_id;
    };
} override_value_t;

typedef struct {
    override_key_t   key;
    override_value_t value;
} override_t;

typedef struct { 
    override_key_t  alt;
    override_key_t  primary;
} alt_override_t;

override_t lookup_override_by_key(override_key_t key);
override_t lookup_override(z64_actor_t *actor, uint8_t scene, uint8_t item_id);
override_key_t resolve_alternative_override(override_key_t override_key);
bool should_override_collectible(EnItem00 *this);
void Collectible_WaitForMessageBox(EnItem00 *this, z64_game_t *game);
void reset_collectible_mutex();
void override_flags_init();
bool Get_CollectibleOverrideFlag(EnItem00 *item00);
uint8_t get_enemy_flag_offset(uint8_t scene, uint8_t room);

#endif
