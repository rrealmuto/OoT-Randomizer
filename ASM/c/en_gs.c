#include "z64.h"
#include "en_gs.h"
#include "get_items.h"

typedef void (*z64_SetOcarinaActionFunc)(z64_game_t* globalCtx, uint16_t ocarinaMode);
#define z64_SetOcarinaAction ((z64_SetOcarinaActionFunc)z64_SetOcarinaAction_Addr)
#define z64_SetOcarinaAction_Addr 0x800DD400

override_t get_gossipstone_override(z64_actor_t *actor, z64_game_t *game) {
    // make a dummy EnItem00 with enough info to get the override
    EnItem00 dummy;
    dummy.actor.actor_id = 0x15;
    dummy.actor.rot_init.y = actor->rot_init.z;
    dummy.actor.variable = 0;

    override_t override = lookup_override(&(dummy.actor), game->scene_index, 0);
    if(override.key.all != 0)
    {
        dummy.override = override;
        if(!Get_CollectibleOverrideFlag(&dummy))
        {
            return override;
        }
    }
    return (override_t) { 0 };
}

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
                        if(globalCtx->msgContext.unk_E3F2 == OCARINA_SONG_TIME && get_gossipstone_override(this, globalCtx).key.all != 0)
                        {
                            drop_collectible_override_flag = this->actor.rot_init.z;
                            EnItem00* spawned = z64_Item_DropCollectible(globalCtx, &(this->actor.pos_world), ITEM00_RUPEE_GREEN);
                            drop_collectible_override_flag = 0;
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
