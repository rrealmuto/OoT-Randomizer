#include "z64.h"
#include "en_item00.h"

// EnItem00 Action Function used for sending outgoing junk overrides collected from enitem00 collectibles
void EnItem00_OutgoingAction(EnItem00* this, z64_game_t* globalCtx) {
    z64_link_t* player = &z64_link;

    if (this->timeToLive == 0) {
        z64_ActorKill(&this->actor);
        return;
    }

    this->actor.pos_world = player->common.pos_world;

    this->actor.rot_2.y += 960;

    this->actor.pos_world.y += 40.0f + (15 - this->timeToLive)*5.0; //+ z64_Math_SinS(this->timeToLive * 15000) * (this->timeToLive * 0.3f);

    if (LINK_IS_ADULT) {
        this->actor.pos_world.y += 20.0f;
    }
}
