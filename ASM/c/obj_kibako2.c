#include "obj_kibako2.h"


//Hacks the regular crate spawn collectible function to use more flag space
//The additional flag info is stored in the actors dropFlag variable (unused by collectibles)

void ObjKibako2_SpawnCollectible_Hack(ObjKibako2* this, z64_game_t* globalCtx) {
    int16_t itemDropped;
    int16_t collectibleFlagTemp;

    collectibleFlagTemp = this->collectibleFlag & 0x3F; //Get the vanilla part of the collectible flag
    uint16_t extendedCollectibleFlag = (this->collectibleFlag & 0x00C0); //Get the upper part of the collectible flag that we'll store elsewhere
    itemDropped = this->dyna.actor.rot_init.x & 0x1F;
    if (itemDropped >= 0 && itemDropped < 0x1A) {
        EnItem00* spawned = z64_Item_DropCollectible(globalCtx, &this->dyna.actor.pos_2, itemDropped | (collectibleFlagTemp << 8) | extendedCollectibleFlag);
        //spawned->actor.dropFlag |= (extendedCollectibleFlag << 1) & 0xFE; 
    }
}