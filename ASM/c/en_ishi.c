#include "z64.h"

int32_t EnIshi_OfferGetItem_Hook(z64_actor_t* actor, z64_game_t* globalCtx, int32_t getItemId, float xzRange, float yRange) {
    // Calculate xz range based on object's scale
    // large rock vanilla scale is 0.4

    float scale = actor->scale.x / 0.4f;
    xzRange = xzRange * scale;
    return Actor_OfferGetItem(actor, globalCtx, getItemId, xzRange, yRange);
}
