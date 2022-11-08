#include "z64.h"
#include "get_items.h"
static colorRGBA8_t sEffectPrimColor = { 255, 255, 127, 0 };
static colorRGBA8_t sEffectEnvColor = { 255, 255, 255, 0 };
static z64_xyzf_t sEffectVelocity = { 0.0f, 0.1f, 0.0f };
static z64_xyzf_t sEffectAccel = { 0.0f, 0.01f, 0.0f };

extern uint8_t ENEMY_DROP_SHUFFLE;

void bb_red_wait_hack(z64_actor_t* actor)
{
    uint16_t flag = actor->rot_init.z;
    if(ENEMY_DROP_SHUFFLE && flag > 0)
    {
        flag -= 1;
        uint16_t params = ((flag & 0x3F) << 8) + (flag & 0xC0) + 0x0001 + 0x0020;

        EnItem00 dummy;
        dummy.collectibleFlag = (params & 0x3F00) >> 8;
        dummy.actor.actor_id = 0x15;
        dummy.actor.dropFlag = 0x80;
        dummy.actor.dropFlag |= (params & 0x00C0) >> 5;
        dummy.actor.variable = params;

        if(should_override_collectible(&dummy))
        {
            z64_xyzf_t pos;
            pos.x = actor->pos_init.x;
            pos.y = actor->pos_init.y + 150.0;
            pos.z = actor->pos_init.z;
            z64_EffectSsKiraKira_SpawnSmall(&z64_game, &pos, &sEffectVelocity, &sEffectAccel, &sEffectPrimColor, &sEffectEnvColor );
        }
        
    }
    
}