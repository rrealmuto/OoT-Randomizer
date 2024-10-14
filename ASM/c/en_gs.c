#include "z64.h"
#include "en_gs.h"
#include "get_items.h"
#include "actor.h"

typedef void (*z64_SetOcarinaActionFunc)(z64_game_t* globalCtx, uint16_t ocarinaMode);
#define z64_SetOcarinaAction ((z64_SetOcarinaActionFunc)z64_SetOcarinaAction_Addr)
#define z64_SetOcarinaAction_Addr 0x800DD400

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
                if ((globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_SARIAS) ||
                    (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_EPONAS) ||
                    (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_LULLABY) ||
                    (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_SUNS) ||
                    (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_TIME)) {
                        xflag_t* flag = &(Actor_GetAdditionalData(this)->flag);
                        if(globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_TIME && flag->all && !Get_NewFlag(flag))
                        {
                            xflag_t* flag = &(Actor_GetAdditionalData(&this->actor)->flag);
                            drop_collectible_override_flag = *flag;
                            EnItem00* spawned = z64_Item_DropCollectible(globalCtx, &(this->actor.pos_world), ITEM00_RUPEE_GREEN);
                            z64_bzero(&drop_collectible_override_flag, sizeof(drop_collectible_override_flag));
                        }
                        else
                        {
                            z64_SpawnActor(&globalCtx->actor_ctxt, globalCtx, ACTOR_EN_ELF, this->actor.pos_world.x,
                                this->actor.pos_world.y + 40.0f, this->actor.pos_world.z, 0, 0, 0, FAIRY_HEAL_TIMED);
                        }

                    //Audio_PlayActorSound2(&this->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                } else if (globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_STORMS) {
                    z64_SpawnActor(&globalCtx->actor_ctxt, globalCtx, ACTOR_EN_ELF, this->actor.pos_world.x,
                                this->actor.pos_world.y + 40.0f, this->actor.pos_world.z, 0, 0, 0, FAIRY_HEAL_BIG);
                    //Audio_PlayActorSound2(&this->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                }
                this->unk_19D = 0;
                z64_Flags_SetSwitch(globalCtx, (this->actor.variable >> 8) & 0x3F);
            } else if (globalCtx->msgContext.ocarinaMode == OCARINA_MODE_01) {
                player->state_flags_2 |= 0x800000;
            }
        }
    }
}
