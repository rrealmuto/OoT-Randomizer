#include "z64.h"
#include "en_gs.h"
#include "get_items.h"
#include "actor.h"
#include "fairy.h"
#include "audio.h"

typedef void (*z64_SetOcarinaActionFunc)(z64_game_t* globalCtx, uint16_t ocarinaMode);
#define z64_SetOcarinaAction ((z64_SetOcarinaActionFunc)z64_SetOcarinaAction_Addr)
#define z64_SetOcarinaAction_Addr 0x800DD400
#define NA_SE_EV_BUTTERFRY_TO_FAIRY 0x28E7
extern xflag_t* spawn_actor_with_flag;

void En_Gs_Update_Hack(EnGs* this, z64_game_t* globalCtx) {
    z64_link_t* player = &z64_link;

    if(this->actor.xzdist_from_link <= 100.0f) {
        if(this->unk_19D == 0) {
            player->state_flags_2 |= 0x800000;
            if(player->state_flags_2 & 0x1000000) {
                z64_SetOcarinaAction(globalCtx, OCARINA_ACTION_FREE_PLAY);
                this->unk_19D |= 1;
            }
        } else if (this->unk_19D & 1) {
            if (globalCtx->msgContext.ocarinaMode == OCARINA_MODE_04) {
                ActorAdditionalData* extras = Actor_GetAdditionalData(&this->actor);
                xflag_t flag = { 0 };
                Actor_BuildFlag(&this->actor, &flag, extras->actor_id, 0);
                override_t override = { 0 };
                bool spawn = false;
                int16_t spawn_params;
                if ((globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_SARIAS) ||
                    (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_EPONAS) ||
                    (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_LULLABY) ||
                    (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_SUNS) ||
                    (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_TIME)) {
                        spawn = true;
                        flag.subflag = 1;
                        spawn_params = FAIRY_HEAL_TIMED;

                    Actor_PlaySfx(&this->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                } else if (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_STORMS) {
                    spawn = true;
                    flag.subflag = 2;
                    spawn_params = FAIRY_HEAL_BIG;
                    Actor_PlaySfx(&this->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                }
                if(spawn) {
                    flag = resolve_alternative_flag(&flag);
                    if(!Get_NewFlag(&flag))
                    {
                        override = get_newflag_override(&flag);
                        spawn_params = FAIRY_HEAL;
                    }
                    spawn_actor_with_flag = &flag;
                    EnElf* spawned = (EnElf*)z64_SpawnActor(&globalCtx->actor_ctxt, globalCtx, ACTOR_EN_ELF, this->actor.pos_world.x,
                                this->actor.pos_world.y + 40.0f, this->actor.pos_world.z, 0, 0, 0, spawn_params);
                    spawn_actor_with_flag = NULL;
                    if(spawned) {
                        spawned->override = override;
                    }
                }
                    
                this->unk_19D = 0;
                // Only set the switch if we didn't override
                if(spawn && !override.key.all)
                    z64_Flags_SetSwitch(globalCtx, (this->actor.variable >> 8) & 0x3F);
            } else if (globalCtx->msgContext.ocarinaMode == OCARINA_MODE_01) {
                player->state_flags_2 |= 0x800000;
            }
        }
    }
}
