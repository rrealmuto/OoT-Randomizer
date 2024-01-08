#include <stdbool.h>
#include "models.h"
#include "get_items.h"
#include "item_table.h"
#include "item_draw_table.h"
#include "util.h"
#include "z64.h"
#include "shop_actors.h"
#include "actor.h"

#define slot_count 24
#define object_size 0x1E70
#define num_vanilla_objects 0x192


extern uint8_t SHUFFLE_CHEST_GAME;
extern z64_object_table_t EXTENDED_OBJECT_TABLE[];
extern EnItem00* collectible_mutex;

loaded_object_t object_slots[slot_count] = { 0 };

z64_object_table_t* get_object_entry(uint32_t object_id) {
    if (object_id <= num_vanilla_objects) {
        return &(z64_object_table[object_id]);
    } else {
        return &EXTENDED_OBJECT_TABLE[object_id - num_vanilla_objects - 1];
    }
}

uint32_t get_object_size(uint32_t object_id) {
    z64_object_table_t *entry = get_object_entry(object_id);
    uint32_t vrom_start = entry->vrom_start;
    uint32_t size = entry->vrom_end - vrom_start;
    return size;
}

uint32_t load_object_file(uint32_t object_id, uint8_t *buf) {
    z64_object_table_t *entry = get_object_entry(object_id);
    uint32_t vrom_start = entry->vrom_start;
    uint32_t size = entry->vrom_end - vrom_start;
    read_file(buf, vrom_start, size);
    return size;
}

void load_object(loaded_object_t* object, uint32_t object_id) {
    object->object_id = object_id;
    load_object_file(object_id, object->buf);
}

loaded_object_t* get_object(uint32_t object_id) {
    for (int i = 0; i < slot_count; i++) {
        loaded_object_t* object = &(object_slots[i]);
        if (object->object_id == object_id) {
            return object;
        }
        if (object->object_id == 0) {
            load_object(object, object_id);
            return object;
        }
    }

    return NULL;
}

void set_object_segment(loaded_object_t* object) {
    z64_disp_buf_t* xlu = &(z64_ctxt.gfx->poly_xlu);
    gSPSegment(xlu->p++, 6, (uint32_t)(object->buf));

    z64_disp_buf_t* opa = &(z64_ctxt.gfx->poly_opa);
    gSPSegment(opa->p++, 6, (uint32_t)(object->buf));
}

void scale_top_matrix(float scale_factor) {
    float* matrix = z64_GetMatrixStackTop();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            matrix[4*i + j] *= scale_factor;
        }
    }
}

typedef void (*pre_draw_fn)(z64_actor_t* actor, z64_game_t* game, uint32_t unknown);
typedef void (*actor_draw_fn)(z64_actor_t* actor, z64_game_t* game);
#define pre_draw_1 ((pre_draw_fn)0x80022438)
#define pre_draw_2 ((pre_draw_fn)0x80022554)
#define base_collectable_draw ((actor_draw_fn)0x80013268)

void draw_model_low_level(uint8_t graphic_id_minus_1, z64_actor_t* actor, z64_game_t* game) {
    pre_draw_1(actor, game, 0);
    pre_draw_2(actor, game, 0);
    base_draw_gi_model(game, graphic_id_minus_1);
}

float scale_factor(uint8_t graphic_id, z64_actor_t* actor, float base_scale) {
    if (graphic_id == 0x63) {
        // Draw skull tokens at their vanilla size
        return base_scale * 0.5;
    }
    if (actor->actor_id == 0xF1 && (graphic_id == 0x46 || graphic_id == 0x2F)) {
        // Draw ocarinas in the moat at vanilla size
        return 1.0;
    }
    return base_scale;
}

void draw_model(model_t model, z64_actor_t* actor, z64_game_t* game, float base_scale) {
    loaded_object_t* object = get_object(model.object_id);
    if (object != NULL) {
        set_object_segment(object);
        if (base_scale != 0.0) {
            scale_top_matrix(scale_factor(model.graphic_id, actor, base_scale));
        }
        draw_model_low_level(model.graphic_id - 1, actor, game);
    }
}

void models_init() {
    for (int i = 0; i < slot_count; i++) {
        object_slots[i].object_id = 0;
        object_slots[i].buf = heap_alloc(object_size);
    }
}

void models_reset() {
    for (int i = 0; i < slot_count; i++) {
        object_slots[i].object_id = 0;
    }
}

void lookup_model_by_override(model_t* model, override_t override) {
    if (override.key.all != 0) {
        override_t model_override = override;
        model_override.value.base.item_id = override.value.looks_like_item_id ?
            override.value.looks_like_item_id :
            override.value.base.item_id;
        uint16_t resolved_item_id = resolve_upgrades(model_override);
        item_row_t* item_row = get_item_row(resolved_item_id);
        model->object_id = item_row->object_id;
        model->graphic_id = item_row->graphic_id;
    }
}

void lookup_model(model_t* model, z64_actor_t* actor, z64_game_t* game, uint16_t base_item_id) {
    override_t override = lookup_override(actor, game->scene_index, base_item_id);
    lookup_model_by_override(model, override);
}

// Shop draw function for each shelf slot, replaces GetItem_Draw inside of EnGirlA_Draw
void shop_draw(z64_actor_t* actor, z64_game_t* game) {
    EnGirlA* this = (EnGirlA*)actor;
    model_t model = {
        .object_id = 0x0000,
        .graphic_id = 0x00,
    };
    override_t override = lookup_override((z64_actor_t*) this, z64_game.scene_index, this->getItemId);

    /*
        SOLD OUT is given a get item ID of 0x53 for the slot,
        which conflicts with the Gerudo Mask override if it's
        shuffled and the override in the Gerudo Mask slot happens
        to be progressive. To prevent the mask shop from filling up
        with longshots or golden gauntlets, check the currently loaded
        object ID for OBJECT_GI_SOLDOUT (0x148) before attempting to use
        the override model.
    */
    if (override.key.all && this->getItemId && game->obj_ctxt.objects[this->objBankIndex].id != 0x148) {
        lookup_model_by_override(&model, override);
        if (model.object_id != 0x0000) {
            draw_model(model, actor, game, 0.0);
        }
    } else {
        // vanilla draw function if the slot is a regular shop item, shuffled or unshuffled
        GetItem_Draw(game, this->giDrawId);
    }
}

void heart_piece_draw(z64_actor_t* actor, z64_game_t* game) {
    model_t model = {
        .object_id = 0x00BD,
        .graphic_id = 0x14,
    };
    lookup_model(&model, actor, game, 0);
    draw_model(model, actor, game, 25.0);
}

// collectible draw function for common items (sticks, nuts, arrows/seeds/etc. and keys)
void collectible_draw_other(z64_actor_t* actor, z64_game_t* game) {
    EnItem00* this = (EnItem00*)actor;

    model_t model = {
        .object_id = 0x0000,
        .graphic_id = 0x00,
    };

    // Handle keys separately for now.
    int collectible_type = actor->variable & 0xFF;
    if (collectible_type == 0x11) {
        lookup_model(&model, actor, game, 0);
        draw_model(model, actor, game, 10.0);
        return;
    }
    base_collectable_draw(actor,game);
}

void heart_container_draw(z64_actor_t* actor, z64_game_t* game) {
    model_t model = {
        .object_id = 0x00BD,
        .graphic_id = 0x13,
    };
    lookup_model(&model, actor, game, 0x4F);
    draw_model(model, actor, game, 1.25);
}

void skull_token_draw(z64_actor_t* actor, z64_game_t* game) {
    model_t model = {
        .object_id = 0x015C,
        .graphic_id = 0x63,
    };
    lookup_model(&model, actor, game, 0);
    draw_model(model, actor, game, 2.0);
}

void ocarina_of_time_draw(z64_actor_t* actor, z64_game_t* game) {
    model_t model = {
        .object_id = 0x00DE,
        .graphic_id = 0x2F,
    };
    lookup_model(&model, actor, game, 0x0C);
    draw_model(model, actor, game, 2.5);
}

void item_etcetera_draw(z64_actor_t* actor, z64_game_t* game) {
    override_t override = { 0 };
    if (actor->variable == 0x01) {
        // Ruto's Letter
        override = lookup_override(actor, game->scene_index, 0x15);
    } else if (actor->variable == 0x07) {
        // Fire Arrow
        override = lookup_override(actor, game->scene_index, 0x58);
    } else if (actor->variable == 0x0A0C) {
        // Treasure chest game heart piece inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x0A,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x0008 && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 1 Bottom Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x00,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x010D && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 1 Top Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x01,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x0208 && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 2 Bottom Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x02,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x030D && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 2 Top Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x03,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x0409 && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 3 Bottom Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x04,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x050D && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 3 Top Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x05,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x0609 && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 4 Bottom Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x06,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x070D && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 4 Top Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x07,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x080A && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 5 Bottom Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x08,
        };
        override = lookup_override_by_key(key);
    } else if (actor->variable == 0x090D && SHUFFLE_CHEST_GAME) {
        //Chest Game Room 5 Top Chest item inside chest
        override_key_t key = {
            .scene = 0x10,
            .type = OVR_CHEST,
            .flag = 0x09,
        };
        override = lookup_override_by_key(key);
    }

    model_t model = { 0 };
    lookup_model_by_override(&model, override);
    if (model.object_id != 0) {
        draw_model(model, actor, game, 1.0);
    } else {
        uint8_t default_graphic_id = *(((uint8_t*)actor) + 0x141);
        draw_model_low_level(default_graphic_id, actor, game);
    }
}

void bowling_bomb_bag_draw(z64_actor_t* actor, z64_game_t* game) {
    override_t override = { 0 };
    switch (actor->variable) {
        case 0x00:
        case 0x05: // bomb bag
            override = lookup_override(actor, game->scene_index, 0x34);
            break;
        case 0x01:
        case 0x06: // heart piece
            override = lookup_override(actor, game->scene_index, 0x3E);
            break;
        case 0x02:
        case 0x07: // bombchus
            override = lookup_override(actor, game->scene_index, 0x03);
            break;
        case 0x03:
        case 0x08: // bombs
            override = lookup_override(actor, game->scene_index, 0x65);
            break;
        case 0x04:
        case 0x09: // purple rupee
            override = lookup_override(actor, game->scene_index, 0x55);
            break;
    }

    model_t model = { 0 };
    lookup_model_by_override(&model, override);
    if (model.object_id != 0) {
        draw_model(model, actor, game, 1.0);
    } else {
        uint8_t default_graphic_id = *(((uint8_t*)actor) + 0x147);
        draw_model_low_level(default_graphic_id, actor, game);
    }
}

void bowling_heart_piece_draw(z64_actor_t* actor, z64_game_t* game) {
    model_t model = {
        .object_id = 0x00BD,
        .graphic_id = 0x14,
    };
    lookup_model(&model, actor, game, 0x3E);
    draw_model(model, actor, game, 1.0);
}
