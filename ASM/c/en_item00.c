#include "z64.h"
#include "get_items.h"

bool EnItem00_ProximityCheck_Hack(EnItem00* this, z64_game_t* GlobalCtx){
    if (this->is_silver_rupee) {
        if (this->actor.distsq_from_link <= 900.0)
        {
            return true;
        }
    }
    else {
        if (((this->actor.xzdist_from_link <= 30.0f) && (this->actor.ydist_from_link >= -50.0f) && (this->actor.ydist_from_link <= 50.0f))) {
            return true;
        }
    }
    return false;
}