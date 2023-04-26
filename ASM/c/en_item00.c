#include "z64.h"
#include "get_items.h"

// Hack to EnItem00_Update when it is checking if the player is in proximity to give the item.
// With silver rupee shuffle, silver rupees actors (En_G_Switch) are replaced by En_Item00 actors.
// These actors perform their proximity checks slightly different from one another.
// This function checks whether or not the EnItem00 is a hacked silver rupee and performs the appropriate proximity check.
// Returns true if the player is within proximity to collect the item.
bool EnItem00_ProximityCheck_Hack(EnItem00* this, z64_game_t* GlobalCtx) {
    if (this->is_silver_rupee) {
        if (this->actor.distsq_from_link <= 900.0) {
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
