#ifndef AUDIO_H
#define AUDIO_H

#include "z64_math.h"
#include <stdint.h>
#include "z64.h"

extern Vec3f z64_SfxDefaultPos;
extern float z64_SfxDefaultFreqAndVolScale;
extern uint8_t z64_SfxDefaultReverb;
extern void Actor_PlaySfx(z64_actor_t* actor, uint16_t sfxId);
extern void Audio_PlaySfxGeneral(uint16_t sfxId, Vec3f* pos, uint8_t token, float* freqScale, float* a4, int8_t* reverbAdd);

#define SFX_PLAY_CENTERED(sfxId)                                                                              \
    Audio_PlaySfxGeneral(sfxId, &z64_SfxDefaultPos, 4, &z64_SfxDefaultFreqAndVolScale, &z64_SfxDefaultFreqAndVolScale, \
                         &z64_SfxDefaultReverb);


extern uint16_t GET_ITEM_SEQ_ID;

#endif //AUDIO_H
