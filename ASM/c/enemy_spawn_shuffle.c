#include "enemy_spawn_shuffle.h"
#include "z64.h"
#include "save.h"
#include "util.h"

uint8_t CFG_ENEMY_SPAWN_SHUFFLE = 0;
bool curr_room_enemies_inhibited = false; //When loading a room, set to keep track if any enemies were prevented from spawning due to enemy spawn shuffle. Used when checking for room clear to prevent clear if enemies are inhibited.

enemy_spawn_table_entry enemy_spawn_table[] = {
    ENEMY_SPAWN_TABLE_ENTRY(0x0002, 0,  NULL), //En_Test (Stalfos)
    ENEMY_SPAWN_TABLE_ENTRY(0x000E, 1,  NULL), //En_Okuta (Octorok (and big octo))
    ENEMY_SPAWN_TABLE_ENTRY(0x00C8, 1,  spawn_check_big_octo), //Jabu objects Big Octo Platform
    ENEMY_SPAWN_TABLE_ENTRY(0x0011, 2,  NULL), //En_Wallmas (Wallmaster)
    ENEMY_SPAWN_TABLE_ENTRY(0x0012, 3,  NULL), //En_Dodongo (Dodongo)
    ENEMY_SPAWN_TABLE_ENTRY(0x0013, 4,  NULL), //En_Firefly (Keese (fire, ice, regular))
    ENEMY_SPAWN_TABLE_ENTRY(0x001B, 5,  NULL), //En_Tite (Tektite)
    ENEMY_SPAWN_TABLE_ENTRY(0x001D, 6,  NULL), //En_Peehat (Peahat)
    ENEMY_SPAWN_TABLE_ENTRY(0x0025, 7,  NULL), //En_Zf (Lizalfos/Dinalfos)
    ENEMY_SPAWN_TABLE_ENTRY(0x002B, 8,  NULL), //En_Goma (Gohma Larva)
    ENEMY_SPAWN_TABLE_ENTRY(0x002D, 9,  NULL), //En_Bubble (Shabom)
    ENEMY_SPAWN_TABLE_ENTRY(0x002F, 10, NULL), //En_Dodojr (Baby Dodongo)
    ENEMY_SPAWN_TABLE_ENTRY(0x0034, 11, NULL), //En_Bili (Biri/Bari)
    ENEMY_SPAWN_TABLE_ENTRY(0x0063, 11, NULL), //Bari
    ENEMY_SPAWN_TABLE_ENTRY(0x0035, 12, NULL), //En_Tp (Tailpasarn)
    ENEMY_SPAWN_TABLE_ENTRY(0x0037, 13, NULL), //En_St (Skulltula)
    ENEMY_SPAWN_TABLE_ENTRY(0x0038, 14, NULL), //En_Bw (Torch Slug)
    ENEMY_SPAWN_TABLE_ENTRY(0x004B, 15, NULL), //En_Mb (Moblin)
    ENEMY_SPAWN_TABLE_ENTRY(0x0054, 16, spawn_check_armos), //En_Am (Armos)
    ENEMY_SPAWN_TABLE_ENTRY(0x0055, 17, NULL), //En_Dekubaba (Deku Baba)
    ENEMY_SPAWN_TABLE_ENTRY(0x00C7, 17, NULL), //Withered Deku Baba
    ENEMY_SPAWN_TABLE_ENTRY(0x0060, 18, NULL), //En_Dekunuts, En_Hintnuts (Deku Scrub)
    ENEMY_SPAWN_TABLE_ENTRY(0x0192, 18, NULL), //En_Hintnuts (Hint Scrub from Deku Tree)
    ENEMY_SPAWN_TABLE_ENTRY(0x0069, 19, NULL), //En_Bb (Bubble (fire, blue, white))
    ENEMY_SPAWN_TABLE_ENTRY(0x008A, 20, NULL), //En_Vm (Beamos)
    ENEMY_SPAWN_TABLE_ENTRY(0x008E, 21, NULL), //En_Floormas (Floormaster (maybe combine))
    ENEMY_SPAWN_TABLE_ENTRY(0x0090, 22, NULL), //En_Rd (Redead/Gibdo)
    ENEMY_SPAWN_TABLE_ENTRY(0x0095, 23, spawn_check_gs), //En_Sw (Skullwalltula (but not gold skulltula))
    ENEMY_SPAWN_TABLE_ENTRY(0x0099, 24, NULL), //En_Fd (Flare Dancer)
    ENEMY_SPAWN_TABLE_ENTRY(0x00A4, 25, NULL), //En_Dh (Dead hand)
    ENEMY_SPAWN_TABLE_ENTRY(0x00C5, 26, NULL), //En_Sb (Shell blade)
    ENEMY_SPAWN_TABLE_ENTRY(0x00DD, 27, NULL), //En_Rr (Like-like)
    ENEMY_SPAWN_TABLE_ENTRY(0x00EC, 28, NULL), //En_Ny (spike)
    ENEMY_SPAWN_TABLE_ENTRY(0x00F6, 29, NULL), //En_Anubice_Tag, En_Anubice (Anubis (spawner))
    ENEMY_SPAWN_TABLE_ENTRY(0x0113, 30, NULL), //En_Ik (Iron Knuckle)
    ENEMY_SPAWN_TABLE_ENTRY(0x0115, 31, spawn_check_skullkid), //En_Skj (Skull Kid)
    ENEMY_SPAWN_TABLE_ENTRY(0x011D, 32, NULL), //En_Tubo_Trap (Flying Pot)
    ENEMY_SPAWN_TABLE_ENTRY(0x0121, 33, NULL), //En_Fz (Freezard (Frezzard))
    ENEMY_SPAWN_TABLE_ENTRY(0x018C, 34, NULL), //En_Weiyer, En_Eiyer (Stinger and Water Stinger)
    ENEMY_SPAWN_TABLE_ENTRY(0x003A, 34, NULL), //En_Eiyer (Stinger)
    ENEMY_SPAWN_TABLE_ENTRY(0x01AF, 35, NULL), //En_Wf (Wolfos)
    ENEMY_SPAWN_TABLE_ENTRY(0x01C0, 36, NULL), //En_Crow (Guay)
    ENEMY_SPAWN_TABLE_ENTRY(0x0028, 37, NULL), //Boss_Goma (Queen Gohma)
    ENEMY_SPAWN_TABLE_ENTRY(0x0027, 38, NULL), //Boss_Dodongo (King Dodongo)
    ENEMY_SPAWN_TABLE_ENTRY(0x00BA, 39, NULL), //Boss_Va (Barinade)
    ENEMY_SPAWN_TABLE_ENTRY(0x0052, 40, NULL), //Boss_Ganondrof (Phantom Ganon)
    ENEMY_SPAWN_TABLE_ENTRY(0x0096, 41, NULL), //Boss_Fd (Volvagia)
    ENEMY_SPAWN_TABLE_ENTRY(0x00C4, 42, NULL), //Boss_Mo (Morpha)
    ENEMY_SPAWN_TABLE_ENTRY(0x00E9, 43, NULL), //Boss_Sst (Bongo Bongo)
    ENEMY_SPAWN_TABLE_ENTRY(0x00DC, 44, NULL), //Boss_Tw (Twinrova)
    ENEMY_SPAWN_TABLE_ENTRY(0x00DE, 45, NULL) //En_Ba (Jabu Jabu Tentacles),

};

// Return false if this is a gold skulltula
bool spawn_check_gs(ActorEntry *actorEntry, z64_game_t *globalCtx, bool *overridden) {
    uint16_t type = (actorEntry->params & 0xE000) >> 13;
    if (type == 4 || type == 5) // Check if it's a gold skulltula
        return false;
    return true;
}

bool spawn_check_armos(ActorEntry *actorEntry, z64_game_t *globalCtx, bool *overridden) {
    if (actorEntry->params == 0) // Pushable armos statue, don't treat like an enemy
        return false;
    return true;
}

bool spawn_check_big_octo(ActorEntry *actorEntry, z64_game_t *globalCtx, bool *overridden) {
    if ((actorEntry->params & 0x00FF) == 0)
        return true;
    return false;
}

bool spawn_check_skullkid(ActorEntry *actorEntry, z64_game_t *globalCtx, bool *overridden) {
    uint16_t type = (actorEntry->params >> 0x0A) & 0x3F;
    if (type <= 6) // Ocarina game/sarias song skull kids
        return false;
    return true;
}

// Spawn override function for enemy spawn shuffle.
// Check if the actor id is in the enemy_spawn_table, and if it is check if the enemy spawn flag is set in extended save context
bool spawn_override_enemy_spawn_shuffle(ActorEntry *actorEntry, z64_game_t *globalCtx, bool* overridden)
{
    if ( CFG_ENEMY_SPAWN_SHUFFLE ) {
        for (int i = 0; i < array_size(enemy_spawn_table); i++) {
            if (actorEntry->id == enemy_spawn_table[i].actor_id) {
                enemy_spawn_table_entry *table_entry = &(enemy_spawn_table[i]);
                bool continue_spawn = true;
                if (enemy_spawn_table[i].override_func) {
                    if (!enemy_spawn_table[i].override_func(actorEntry, globalCtx, overridden))
                        return true;
                }
                continue_spawn &= (extended_savectx.enemy_spawn_flags[table_entry->index/8] & (1 << (table_entry->index % 8))) > 0; 
                curr_room_enemies_inhibited |= !continue_spawn;
                return continue_spawn;
            }
        }
    }
    
    return true;
}

void Actor_RemoveFromCategory_SetTempClear_Hack(z64_game_t *globalCtx, int32_t flag)
{
    if(curr_room_enemies_inhibited) {
        return;
    }
    z64_Flags_SetTempClear(globalCtx, flag);
}