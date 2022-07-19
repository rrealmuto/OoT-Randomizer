#include "get_items.h"

#include "icetrap.h"
#include "item_table.h"
#include "stdbool.h"
#include "util.h"
#include "z64.h"

extern uint8_t FAST_CHESTS;
extern uint8_t OCARINAS_SHUFFLED;
extern uint8_t NO_COLLECTIBLE_HEARTS;
override_t cfg_item_overrides[1536] = {0};
int item_overrides_count = 0;

override_t pending_item_queue[3] = { 0 };
z64_actor_t *dummy_actor = NULL;

// Co-op state
extern uint8_t PLAYER_ID;
extern uint8_t PLAYER_NAME_ID;
extern uint16_t INCOMING_PLAYER;
extern uint16_t INCOMING_ITEM;
extern override_key_t OUTGOING_KEY;
extern uint16_t OUTGOING_ITEM;
extern uint16_t OUTGOING_PLAYER;

override_t active_override = { 0 };
int active_override_is_outgoing = 0;
item_row_t *active_item_row = NULL;
// Split active_item_row into variables for convenience in ASM
uint32_t active_item_action_id = 0;
uint32_t active_item_text_id = 0;
uint32_t active_item_object_id = 0;
uint32_t active_item_graphic_id = 0;
uint32_t active_item_fast_chest = 0;

uint8_t satisified_pending_frames = 0;

// These tables contain the offset (in words) of the start of the variable-length scene flags in the flag override flag tables for a particular scene.
uint8_t collectible_scene_flags_table[101];
uint8_t dropped_collectible_scene_flags_table[101];

// Total amount of memory required for each flag table (in words).
uint16_t num_override_flags;
uint16_t num_drop_override_flags;

// Pointer to a variable length array that will contain the collectible flags for each scene. Index of each scene in the array stored in the flags_tables above.
uint32_t *collectible_override_flags;
uint32_t *dropped_collectible_override_flags;

// Initialize the override flag tables on the heap.
void override_flags_init() {
    collectible_override_flags = heap_alloc(4 * num_override_flags);
    dropped_collectible_override_flags = heap_alloc(4 * num_drop_override_flags);
}

void item_overrides_init() {
    while (cfg_item_overrides[item_overrides_count].key.all != 0) {
        item_overrides_count++;
    }

    // Create an actor satisfying the minimum requirements to give the player an item
    dummy_actor = heap_alloc(sizeof(z64_actor_t));
    dummy_actor->main_proc = (void *)1;
}

override_key_t get_override_search_key(z64_actor_t *actor, uint8_t scene, uint8_t item_id) {
    if (actor->actor_id == 0x0A) {
        // Don't override WINNER purple rupee in the chest minigame scene
        if (scene == 0x10) {
            int chest_item_id = (actor->variable >> 5) & 0x7F;
            if (chest_item_id == 0x75) {
                return (override_key_t){ .all = 0 };
            }
        }

        return (override_key_t){
            .scene = scene,
            .type = OVR_CHEST,
            .flag = actor->variable & 0x1F,
        };
    } else if (actor->actor_id == 0x15) {
        // Override heart pieces, keys, red, blue, green rupees, and recovery hearts
        int collectable_type = actor->variable & 0xFF;
        if (collectable_type == 0x12)  // don't override fairies
            return (override_key_t){.all = 0};
        /*if (collectable_type != 0x06 && collectable_type != 0x11 && collectable_type != 0x03 && collectable_type != 2 && collectable_type != 0x01 && collectable_type != 0x00 && collectable_type != 13)
        {
            return (override_key_t){.all = 0};
        }*/
        // Check if it was a dropped collectable and use a separate override for that
        EnItem00 *item = (EnItem00 *)actor;
        uint8_t flag = ((item->actor.dropFlag & 0x06) << 5) + item->collectibleFlag;
        if (item->actor.dropFlag) {
            // Use the same override flags for the pots in ganon's tower
            if (scene == 0x0A)
                scene = 0x19;
            return (override_key_t){
                .scene = scene,
                .type = OVR_DROPPEDCOLLECTABLE,
                .flag = flag,
            };
        }

        return (override_key_t){
            .scene = scene,
            .type = OVR_COLLECTABLE,
            .flag = flag};
    } else if (actor->actor_id == 0x19C) {
        return (override_key_t){
            .scene = (actor->variable >> 8) & 0x1F,
            .type = OVR_SKULL,
            .flag = actor->variable & 0xFF,
        };
    } else if (scene == 0x3E && actor->actor_id == 0x011A) {
        return (override_key_t){
            .scene = z64_file.grotto_id,
            .type = OVR_GROTTO_SCRUB,
            .flag = item_id,
        };
    } else {
        return (override_key_t) {
            .scene = scene,
            .type = OVR_BASE_ITEM,
            .flag = item_id,
        };
    }
}

override_t lookup_override_by_key(override_key_t key) {
    int start = 0;
    int end = item_overrides_count - 1;
    while (start <= end) {
        int mid_index = (start + end) / 2;
        override_t mid_entry = cfg_item_overrides[mid_index];
        if (key.all < mid_entry.key.all) {
            end = mid_index - 1;
        } else if (key.all > mid_entry.key.all) {
            start = mid_index + 1;
        } else {
            return mid_entry;
        }
    }
    return (override_t){ 0 };
}

override_t lookup_override(z64_actor_t *actor, uint8_t scene, uint8_t item_id) {
    override_key_t search_key = get_override_search_key(actor, scene, item_id);
    if (search_key.all == 0) {
        return (override_t){ 0 };
    }

    return lookup_override_by_key(search_key);
}

void activate_override(override_t override) {
    uint16_t resolved_item_id = resolve_upgrades(override.value.item_id);
    item_row_t *item_row = get_item_row(resolved_item_id);

    active_override = override;
    if (resolved_item_id == 0xCA)
        active_override_is_outgoing = 2; // Send to everyone
    else
        active_override_is_outgoing = override.value.player != PLAYER_ID;
    active_item_row = item_row;
    active_item_action_id = item_row->action_id;
    active_item_text_id = item_row->text_id;
    active_item_object_id = item_row->object_id;
    active_item_graphic_id = item_row->graphic_id;
    if (override.value.looks_like_item_id) {
        item_row = get_item_row(override.value.looks_like_item_id);
    }
    active_item_fast_chest = item_row->chest_type == BROWN_CHEST || item_row->chest_type == SILVER_CHEST || item_row->chest_type == SKULL_CHEST_SMALL;
    PLAYER_NAME_ID = override.value.player;
}

void clear_override() {
    active_override = (override_t){ 0 };
    active_override_is_outgoing = 0;
    active_item_row = NULL;
    active_item_action_id = 0;
    active_item_text_id = 0;
    active_item_object_id = 0;
    active_item_graphic_id = 0;
    active_item_fast_chest = 0;
}

void set_outgoing_override(override_t *override) {
    OUTGOING_KEY = override->key;
    OUTGOING_ITEM = override->value.item_id;
    OUTGOING_PLAYER = override->value.player;
}

void push_pending_item(override_t override) {
    for (int i = 0; i < array_size(pending_item_queue); i++) {
        if (pending_item_queue[i].key.all == 0) {
            pending_item_queue[i] = override;
            break;
        }
        if (pending_item_queue[i].key.all == override.key.all) {
            // Prevent duplicate entries
            break;
        }
    }
}

void push_coop_item() {
    if (INCOMING_ITEM != 0) {
        override_t override = { 0 };
        override.key.scene = 0xFF;
        override.key.type = OVR_DELAYED;
        override.key.flag = 0xFF;
        override.value.player = INCOMING_PLAYER;
        override.value.item_id = INCOMING_ITEM;
        push_pending_item(override);
    }
}

void push_delayed_item(uint8_t flag) {
    override_key_t search_key = { .all = 0 };
    search_key.scene = 0xFF;
    search_key.type = OVR_DELAYED;
    search_key.flag = flag;
    override_t override = lookup_override_by_key(search_key);
    if (override.key.all != 0) {
        push_pending_item(override);
    }
}

void pop_pending_item() {
    pending_item_queue[0] = pending_item_queue[1];
    pending_item_queue[1] = pending_item_queue[2];
    pending_item_queue[2].key.all = 0;
    pending_item_queue[2].value.all = 0;
}

void after_key_received(override_key_t key) {
    if (key.type == OVR_DELAYED && key.flag == 0xFF) {
        INCOMING_ITEM = 0;
        INCOMING_PLAYER = 0;
        uint16_t *received_item_counter = (uint16_t *)(z64_file_addr + 0x90);
        (*received_item_counter)++;
        return;
    }

    override_key_t fire_arrow_key = {
        .scene = 0x57, // Lake hylia
        .type = OVR_BASE_ITEM,
        .flag = 0x58, // Fire arrows item ID
    };
    if (key.all == fire_arrow_key.all) {
        // Mark fire arrow location as obtained
        z64_game.chest_flags |= 0x1;
    }
}

void pop_ice_trap() {
    override_key_t key = pending_item_queue[0].key;
    override_value_t value = pending_item_queue[0].value;
    if (value.item_id == 0x7C && value.player == PLAYER_ID) {
        push_pending_ice_trap();
        pop_pending_item();
        after_key_received(key);
    }
}

void after_item_received() {
    override_key_t key = active_override.key;
    if (key.all == 0) {
        return;
    }

    if (active_override_is_outgoing) {
        set_outgoing_override(&active_override);
    }

    if (key.all == pending_item_queue[0].key.all) {
        pop_pending_item();
    }
    after_key_received(key);
    clear_override();
}

inline uint32_t link_is_ready() {
    if ((z64_link.state_flags_1 & 0xFCAC2485) == 0 &&
        (z64_link.common.unk_flags_00 & 0x0001) &&
        (z64_link.state_flags_2 & 0x000C0000) == 0 &&
        (z64_event_state_1 & 0x20) == 0 &&
        (z64_game.camera_2 == 0)) {
        satisified_pending_frames++;
    }
    else {
        satisified_pending_frames = 0;
    }
    if (satisified_pending_frames >= 2) {
        satisified_pending_frames = 0;
        return 1;
    }
    return 0;
}

void try_pending_item() {
    override_t override = pending_item_queue[0];

    if(override.key.all == 0) {
        return;
    }

    if (override.value.item_id == 0xCA && override.value.player != PLAYER_ID) {
        uint16_t resolved_item_id = resolve_upgrades(override.value.item_id);
        item_row_t *item_row = get_item_row(resolved_item_id);
        call_effect_function(item_row);
        pop_pending_item();
        after_key_received(override.key);
        clear_override();
        return;
    }

    activate_override(override);

    z64_link.incoming_item_actor = dummy_actor;
    z64_link.incoming_item_id = active_item_row->base_item_id;
}

void handle_pending_items() {
    push_coop_item();
    if (link_is_ready()) {
        pop_ice_trap();
        if (ice_trap_is_pending()) {
            give_ice_trap();
        } else {
            try_pending_item();
        }
    }
}

void get_item(z64_actor_t *from_actor, z64_link_t *link, int8_t incoming_item_id) {
    override_t override = { 0 };
    int incoming_negative = incoming_item_id < 0;

    if (from_actor && incoming_item_id != 0) {
        int8_t item_id = incoming_negative ? -incoming_item_id : incoming_item_id;
        override = lookup_override(from_actor, z64_game.scene_index, item_id);
    }

    if (override.key.all == 0) {
        // No override, use base game's item code
        clear_override();
        link->incoming_item_id = incoming_item_id;
        return;
    }

    activate_override(override);
    int8_t base_item_id = active_item_row->base_item_id;

    if (from_actor->actor_id == 0x0A) {
        // Update chest contents
        if (override.value.item_id == 0x7C && override.value.player == PLAYER_ID && (FAST_CHESTS || active_item_fast_chest)) {
            // Use ice trap base item ID to freeze Link as the chest opens rather than playing the full item get animation
            base_item_id = 0x7C;
        }
        from_actor->variable = (from_actor->variable & 0xF01F) | (base_item_id << 5);
    }

    link->incoming_item_id = incoming_negative ? -base_item_id : base_item_id;
}

#define GIVEITEM_RUPEE_GREEN 0x84
#define GIVEITEM_RUPEE_BLUE 0x85
#define GIVEITEM_RUPEE_RED 0x86
#define GIVEITEM_HEART 0x83
#define GIVEITEM_STICK 0x00
#define GIVEITEM_NUT_5 140
#define GIVEITEM_BOMBS_5 142
#define GIVEITEM_ARROWS_SINGLE 3
#define GIVEITEM_ARROWS_SMALL 146
#define GIVEITEM_ARROWS_MEDIUM 147
#define GIVEITEM_ARROWS_LARGE 148
#define GIVEITEM_SEEDS 88
#define GIVEITEM_MAGIC_SMALL 120
#define GIVEITEM_MAGIC_LARGE 121
#define GIVEITEM_RUPEE_PURPLE 135

#define LEN_ITEMS 21
uint8_t items[] = {
    GIVEITEM_RUPEE_GREEN,
    GIVEITEM_RUPEE_BLUE,
    GIVEITEM_RUPEE_RED,
    GIVEITEM_HEART,
    GIVEITEM_BOMBS_5,
    GIVEITEM_ARROWS_SINGLE,
    0,
    0,
    GIVEITEM_ARROWS_SMALL,
    GIVEITEM_ARROWS_MEDIUM,
    GIVEITEM_ARROWS_LARGE,
    GIVEITEM_BOMBS_5,
    GIVEITEM_NUT_5,
    GIVEITEM_STICK,
    GIVEITEM_MAGIC_LARGE,
    GIVEITEM_MAGIC_SMALL,
    GIVEITEM_SEEDS,
    0,
    0,
    0,
    GIVEITEM_RUPEE_PURPLE};

EnItem00 *collectible_mutex = 0;

override_t collectible_override;

void reset_collectible_mutex() {
    collectible_mutex = NULL;
}

// New EnItem00 function that freezes link until the messagebox is closed. Similar to how skulls work.
void Collectible_WaitForMessageBox(EnItem00 *this, z64_game_t *game) {
    // Check message state:
    if (z64_MessageGetState(((uint8_t *)(&z64_game)) + 0x20D8) == 0) {
        // Make sure link was frozen for the minimum amount of time
        if (this->timeToLive == 0) {
            reset_collectible_mutex();  // release the mutex
            // Kill the actor
            z64_ActorKill(&(this->actor));
        }
    } else {
        z64_link.common.frozen = 10;
    }
}

uint8_t get_extended_flag(EnItem00 *item00) {
    return ((item00->actor.dropFlag & 0xFE) << 5) + item00->collectibleFlag;
}

bool Get_CollectibleOverrideFlag(EnItem00 *item00) {
    uint32_t *flag_table = collectible_override_flags;
    uint8_t *scene_table = &collectible_scene_flags_table[0];
    uint16_t scene = z64_game.scene_index;
    bool dropFlag = item00->actor.dropFlag & 0x0001;

    if (item00->actor.variable == ITEM00_HEART_PIECE || item00->actor.variable == ITEM00_SMALL_KEY || item00->actor.variable == ITEM00_HEART_CONTAINER) {
        return z64_Flags_GetCollectible(&z64_game, item00->collectibleFlag) > 0;
    }

    uint16_t extended_flag = get_extended_flag(item00);
    if (dropFlag)  // we set this if it's dropped
    {
        flag_table = dropped_collectible_override_flags;
        scene_table = &dropped_collectible_scene_flags_table[0];
        if (scene == 0x0A)
            scene = 0x19;
    }

    // uint16_t extended_flag = item00->collectibleFlag; //Update this to make the flag bigger

    return (flag_table[scene_table[scene] + (extended_flag / 0x20)] & (1 << (extended_flag % 0x20)));
    /*
    if(item00->collectibleFlag < 0x20)
    {
        return (flag_table[2*scene] & (1 << item00->collectibleFlag)) > 0;
    }
    return (flag_table[2*scene + 1] & (1 << (item00->collectibleFlag - 0x20))) > 0;
    */
}

void Set_CollectibleOverrideFlag(EnItem00 *item00) {
    uint32_t *flag_table = collectible_override_flags;
    uint8_t *scene_table = &collectible_scene_flags_table[0];
    uint16_t scene = z64_game.scene_index;
    bool dropFlag = item00->actor.dropFlag & 0x0001;
    uint16_t extended_flag = get_extended_flag(item00);
    if (dropFlag) {
        flag_table = dropped_collectible_override_flags;
        scene_table = &dropped_collectible_scene_flags_table[0];
        if (scene == 0x0A)
            scene = 0x19;
    }
    flag_table[scene_table[scene] + (extended_flag / 0x20)] |= (1 << (extended_flag % 0x20));

    // uint16_t extended_flag = item00->collectibleFlag; //Update this to make the flag bigger

    return;
    /*if(item00->collectibleFlag < 0x20)
    {
        flag_table[2 * scene] |= (1 << item00->collectibleFlag);
    }
    else
    {
        flag_table[2 * scene + 1] |= (1 << (item00->collectibleFlag -0x20));
    }*/
}

bool should_override_collectible(EnItem00 *item00) {
    override_t override = lookup_override(&(item00->actor), z64_game.scene_index, 0);
    if (override.key.all == 0 || Get_CollectibleOverrideFlag(item00)) {
        return 0;
    }
    return 1;
}

bool Item00_KillActorIfFlagIsSet(z64_actor_t *actor) {
    EnItem00 *this = (EnItem00 *)actor;
    if (should_override_collectible(this))
        return 0;
    if (get_extended_flag(this) >= 0x40)
        return 0;
    if (z64_Flags_GetCollectible(&z64_game, this->collectibleFlag)) {
        z64_ActorKill(actor);
        return 1;
    }
}

// Hack for keeping freestanding overrides alive when they spawn from crates/pots.
void Item00_KeepAlive(EnItem00 *item00) {
    if (should_override_collectible(item00) && (item00->actionFunc != (EnItem00ActionFunc)0x800127E0)) {
        if (item00->unk_156)
            item00->timeToLive = 0xFF;
    } else {
        if (item00->timeToLive > 0)
            item00->timeToLive--;
    }
}

int16_t get_override_drop_id(int16_t dropId, uint16_t params) {
    // make our a dummy enitem00 with enough info to get the override
    EnItem00 dummy;
    dummy.collectibleFlag = (params & 0x3F00) >> 8;
    dummy.actor.actor_id = 0x15;
    dummy.actor.dropFlag = 1;
    dummy.actor.dropFlag |= (params & 0x00C0) >> 5;
    dummy.actor.variable = dropId;
    if (should_override_collectible(&dummy) &&
        (dropId != ITEM00_HEART_PIECE) &&
        (dropId != ITEM00_SMALL_KEY) &&
        (dropId != ITEM00_HEART_CONTAINER) &&
        (dropId != ITEM00_SHIELD_DEKU) &&
        (dropId != ITEM00_SHIELD_HYLIAN) &&
        (dropId != ITEM00_TUNIC_ZORA) &&
        (dropId != ITEM00_TUNIC_GORON)) {
        dropId = ITEM00_RUPEE_GREEN;
        return dropId;
    }

    if (LINK_IS_ADULT) {
        if (dropId == ITEM00_SEEDS) {
            dropId = ITEM00_ARROWS_SMALL;
        } else if (dropId == ITEM00_STICK) {
            dropId = ITEM00_RUPEE_GREEN;
        }
    } else {
        if (dropId == ITEM00_ARROWS_SMALL || dropId == ITEM00_ARROWS_MEDIUM || dropId == ITEM00_ARROWS_LARGE) {
            dropId = ITEM00_SEEDS;
        }
    }

    // This is convoluted but it seems like it must be a single condition to match
    if (((dropId == ITEM00_BOMBS_A) || (dropId == ITEM00_BOMBS_SPECIAL) || (dropId == ITEM00_BOMBS_B)) && (z64_file.items[ITEM_BOMB] == -1))
        return -1;
    if (((dropId == ITEM00_ARROWS_SMALL) || (dropId == ITEM00_ARROWS_MEDIUM) || (dropId == ITEM00_ARROWS_LARGE)) && (z64_file.items[ITEM_BOW] == -1))
        return -1;
    if (((dropId == ITEM00_MAGIC_LARGE) || (dropId == ITEM00_MAGIC_SMALL)) && (z64_file.magic_capacity_set == 0))
        return -1;
    if (((dropId == ITEM00_SEEDS)) && (z64_file.items[ITEM_SLINGSHOT] == -1)) {
        return -1;
    }

    if ((dropId == ITEM00_HEART) && ((z64_file.energy_capacity == z64_file.energy) || NO_COLLECTIBLE_HEARTS)) {
        return ITEM00_RUPEE_GREEN;
    }

    return dropId;
}

// Override hack for freestanding collectibles (green, blue, red rupees, recovery hearts)
uint8_t item_give_collectible(uint8_t item, z64_link_t *link, z64_actor_t *from_actor) {
    EnItem00 *pItem = (EnItem00 *)from_actor;

    override_t override = lookup_override(from_actor, z64_game.scene_index, 0);

    // Check if we should override the item. We have logic in the randomizer to not include excluded items in the override table.
    if (override.key.all == 0 || Get_CollectibleOverrideFlag(pItem)) {
        z64_GiveItem(&z64_game, items[item]);  // Give the regular item (this is what is normally called by the non-hacked function)
        if (get_extended_flag(pItem) > 0x3F)   // If our extended collectible flag is outside the range of normal collectibles, set the flag to 0 so it doesn't write something wrong. We should only ever be using this for things that normally are 0 anyway
        {
            pItem->collectibleFlag = 0;
            pItem->actor.dropFlag &= 0x01;
        }
        return 0;
    }

    if (!collectible_mutex && pItem->actor.main_proc != NULL)  // Check our mutex so that only one collectible can run at a time (if 2 run on the same frame you lose the message). Also make sure that this actor hasn't already been killed.
    {
        collectible_mutex = (EnItem00*)from_actor;
        collectible_override = override;
        // resolve upgrades and figure out what item to give.
        uint16_t item_id = collectible_override.value.item_id;
        uint16_t resolved_item_id = resolve_upgrades(item_id);
        item_row_t *item_row = get_item_row(resolved_item_id);

        // Set the collectible flag
        Set_CollectibleOverrideFlag(pItem);
        if ((item == ITEM00_HEART_PIECE) || (item == ITEM00_SMALL_KEY))  // Don't allow heart pieces or small keys to be collected a second time. This is really just for the "Drop" types.
            z64_SetCollectibleFlags(&z64_game, pItem->collectibleFlag);
        item_id = collectible_override.value.item_id;
        uint8_t player = collectible_override.value.player;

        PLAYER_NAME_ID = player;

        // If it's a junk item (aka a regular collectible) don't do the fanfare music/message box.
        if (item_row->collectible > 0)  // Item is one of our base collectibles
        {
            collectible_mutex = NULL;
            pItem->actor.health = 1;
            z64_GiveItem(&z64_game, item_row->action_id);
            // pItem->actor.variable = item_row->collectible;
            // Pick the correct sound effect for rupees or other items.
            uint16_t sfxId = NA_SE_SY_GET_ITEM;
            if (item_row->collectible <= ITEM00_RUPEE_RED || item_row->collectible == ITEM00_RUPEE_PURPLE || item_row->collectible == ITEM00_RUPEE_ORANGE)
                sfxId = NA_SE_SY_GET_RUPY;
            z64_Audio_PlaySoundGeneral(sfxId, (void*)0x80104394, 4, (float*)0x801043A0, (float*)0x801043A0, (uint8_t*)0x801043A8);
            return 3;  // Return to the original function so it can draw the collectible above our head.
        }

        // draw message box and play get item sound (like when a skull is picked up)
        z64_Audio_PlayFanFare(NA_BGM_SMALL_ITEM_GET);

        z64_DisplayTextbox(&z64_game, item_row->text_id, 0);

        // Set up
        pItem->timeToLive = 15;  // unk_15A is a frame timer that is decremented each frame by the main actor code.
        pItem->unk_154 = 35;     // not quite sure but this is what the vanilla game does.
        pItem->getItemId = 0;
        z64_link.common.frozen = 10;                        // freeze link (like when picking up a skull)
        pItem->actionFunc = Collectible_WaitForMessageBox;  // Set up the EnItem00 action function to wait for the message box to close.

        // Give the item to the right place
        if (resolved_item_id == 0xCA) {
            // Send triforce to everyone
            set_outgoing_override(&collectible_override);
            z64_GiveItem(&z64_game, item_row->action_id);
            call_effect_function(item_row);
        } else if (player != PLAYER_ID) {
            set_outgoing_override(&collectible_override);
        } else {
            // push_pending_item(override);
            z64_GiveItem(&z64_game, item_row->action_id);
            call_effect_function(item_row);
        }

        return 1;
    }
    return 2;  //
}

void get_skulltula_token(z64_actor_t *token_actor) {
    override_t override = lookup_override(token_actor, 0, 0);
    uint16_t item_id;
    uint8_t player;
    if (override.key.all == 0) {
        // Give a skulltula token if there is no override
        item_id = 0x5B;
        player = PLAYER_ID;
    } else {
        item_id = override.value.item_id;
        player = override.value.player;
    }

    uint16_t resolved_item_id = resolve_upgrades(item_id);
    item_row_t *item_row = get_item_row(resolved_item_id);

    token_actor->draw_proc = NULL;

    PLAYER_NAME_ID = player;
    z64_DisplayTextbox(&z64_game, item_row->text_id, 0);

    if (resolved_item_id == 0xCA) {
        // Send triforce to everyone
        set_outgoing_override(&override);
        z64_GiveItem(&z64_game, item_row->action_id);
        call_effect_function(item_row);
    } else if (player != PLAYER_ID) {
        set_outgoing_override(&override);
    } else {
        z64_GiveItem(&z64_game, item_row->action_id);
        call_effect_function(item_row);
    }
}

int give_sarias_gift() {
    uint16_t received_sarias_gift = (z64_file.event_chk_inf[0x0C] & 0x0002);
    if (received_sarias_gift == 0) {
        if (OCARINAS_SHUFFLED)
            push_delayed_item(0x02);
        z64_file.event_chk_inf[0x0C] |= 0x0002;
    }

    // return 1 to skip the cutscene
    return OCARINAS_SHUFFLED || received_sarias_gift;
}
