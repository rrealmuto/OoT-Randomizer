#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>
#include "z64.h"
#include "z64_math.h"
typedef struct SkelAnime {
    /* 0x00 */ uint8_t limbCount; // Number of limbs in the skeleton
    /* 0x01 */ uint8_t mode; // See `AnimationMode`
    /* 0x02 */ uint8_t dListCount; // Number of display lists in a flexible skeleton
    /* 0x03 */ int8_t taper; // Tapering to use when morphing between animations. Only used by Door_Warp1.
    /* 0x04 */ void** skeleton; // An array of pointers to limbs. Can be StandardLimb, LodLimb, or SkinLimb.
    /* 0x08 */ void* animation; // Can be an AnimationHeader or LinkAnimationHeader.
    /* 0x0C */ float startFrame; // In mode ANIMMODE_LOOP_PARTIAL*, start of partial loop.
    /* 0x10 */ float endFrame; // In mode ANIMMODE_ONCE*, Update returns true when curFrame is equal to this. In mode ANIMMODE_LOOP_PARTIAL*, end of partial loop.
    /* 0x14 */ float animLength; // Total number of frames in the current animation.
    /* 0x18 */ float curFrame; // Current frame in the animation
    /* 0x1C */ float playSpeed; // Multiplied by R_UPDATE_RATE / 3 to get the animation's frame rate.
    /* 0x20 */ z64_xyz_t* jointTable; // Current translation of model and rotations of all limbs
    /* 0x24 */ z64_xyz_t* morphTable; // Table of values used to morph between animations
    /* 0x28 */ float morphWeight; // Weight of the current animation morph as a fraction in [0,1]
    /* 0x2C */ float morphRate; // Reciprocal of the number of frames in the morph
    /* 0x30 */ union {
        int32_t (*normal)(struct SkelAnime*); // Can be Loop, Partial loop, Play once, Morph, or Tapered morph
        int32_t (*link)(z64_game_t*, struct SkelAnime*); // Can be Loop, Play once, or Morph
    } update;
    /* 0x34 */ int8_t initFlags; // Flags used when initializing Link's skeleton
    /* 0x35 */ uint8_t movementFlags; // Flags used for animations that move the actor in worldspace.
    /* 0x36 */ int16_t prevRot; // Previous rotation in worldspace.
    /* 0x38 */ z64_xyz_t prevTransl; // Previous modelspace translation.
    /* 0x3E */ z64_xyz_t baseTransl; // Base modelspace translation.
} SkelAnime; // size = 0x44

#endif