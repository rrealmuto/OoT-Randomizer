#include "triforce.h"
#include "model_text.h"
#include "dungeon_info.h"
#include "file_select.h"
#include "get_items.h"
#include "models.h"
#include "gfx.h"
#include "text.h"
#include "util.h"
#include "dpad.h"
#include "misc_colors.h"
#include "hud_colors.h"
#include "z64.h"
#include "chests.h"
#include "ganon.h"
#include "twinrova.h"
#include "sage_gifts.h"
#include "extern_ctxt.h"
#include "weather.h"
#include "textures.h"
#include "scene.h"
#include "music.h"
#include "uninvertYaxis.h"
#include "debug.h"
#include "enemy_spawn_shuffle.h"
#include "ovl_kaleidoscope.h"
#include "objects.h"
#include "inputviewer.h"
#include "message.h"

void Gameplay_InitSkybox(z64_game_t* globalCtx, int16_t skyboxId);

void c_init() {
    heap_init();
    gfx_init();
    init_textures();
    models_reset();
    override_flags_init();
    reset_collectible_mutex();
    extended_objects_reset();
    item_overrides_init();
    init_new_menus();
    extended_objects_init();
}

void before_game_state_update() {
    rando_display_buffer_reset();
    handle_pending_items();
    handle_dpad();
    update_misc_colors();
    update_hud_colors();
    process_extern_ctxt();
    manage_music_changes();
    manage_uninvert_yaxis();
    display_misc_messages();
}

void after_game_state_update() {
    // Checks if the prerender screen is being drawn before drawing new HUD things.
    // Else this will cause graphical and/or lag issues on some emulators when pausing.
    if (R_PAUSE_BG_PRERENDER_STATE != PAUSE_BG_PRERENDER_PROCESS) {
        draw_dungeon_info(&rando_overlay_db);
        draw_triforce_count(&rando_overlay_db);
        draw_boss_key(&z64_game, &rando_overlay_db);
        draw_silver_rupee_count(&z64_game, &rando_overlay_db);
        draw_illegal_model_text(&rando_overlay_db);
        draw_input_viewer(&rando_overlay_db);
        display_song_name(&rando_overlay_db);
#if DEBUG_MODE
        debug_utilities(&debug_db);
#endif
    }
    close_rando_display_buffer();
    give_sage_gifts();
}

void before_skybox_init(z64_game_t* game, int16_t skyboxId) {
    override_weather_state();
    Gameplay_InitSkybox(game, skyboxId);
}

void after_scene_init() {
    check_ganon_entry();
    clear_twinrova_vars();
    models_reset();
    extern_scene_init();
    check_model_skeletons();
    reset_collectible_mutex();
    get_current_scene_setup_number();
}

extern void Play_Init(z64_game_t*);

// Hooked Play_Init entry point which is called at the beginning of every scene change
// This is where we should reset heap because ZeldaArena is reinitialized every time
void Play_Init_Hook(z64_game_t* this) {
    
    extended_objects_reset();
    Play_Init(this);
    
    
}