#include <stdint.h>
#include "z64.h"
#include "en_wonderitem.h"

static colorRGBA8_t sEffectPrimColor = { 255, 0, 0, 0 };
static colorRGBA8_t sEffectEnvColor = { 255, 255, 255, 0 };
static z64_xyzf_t sEffectVelocity = { 0.0f, 0.1f, 0.0f };
static z64_xyzf_t sEffectAccel = { 0.0f, 0.01f, 0.0f };

void EnWonderItem_Multitag_DrawHack(z64_xyzf_t* tags, uint32_t index)
{
    z64_xyzf_t pos = tags[index];
    z64_EffectSsKiraKira_SpawnSmall(&z64_game, &pos, &sEffectVelocity, &sEffectAccel, &sEffectPrimColor, &sEffectEnvColor );
}