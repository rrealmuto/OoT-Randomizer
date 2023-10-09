#ifndef __AUDIOTOOLS_H__
#define __AUDIOTOOLS_H__

#include <ext_type.h>
#include "AudioConvert.h"

extern char* gTableDesignIteration;
extern char* gTableDesignFrameSize;
extern char* gTableDesignBits;
extern char* gTableDesignOrder;
extern char* gTableDesignThreshold;

void AudioTools_RunTableDesign(AudioSample* sampleInfo);
void AudioTools_RunVadpcmEnc(AudioSample* sampleInfo);

void AudioTools_TableDesign(AudioSample* sampleInfo);
void AudioTools_VadpcmEnc(AudioSample* sampleInfo);
void AudioTools_VadpcmDec(AudioSample* sampleInfo);

void AudioTools_LoadCodeBook(AudioSample* sampleInfo, const char* file);

#endif
