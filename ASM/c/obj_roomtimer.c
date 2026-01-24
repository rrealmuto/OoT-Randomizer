#include "obj_roomtimer.h"
#include "util.h"

extern void OVL_ObjRoomtimer_MainAction(ObjRoomtimer* this, z64_game_t* play);

// Replaces the call to Flags_GetTempClear in the main ObjRoomtimer action function
// Return 1 to finish the timer, 0 to not
int32_t ObjRoomTimer_ConditionCheckHack(ObjRoomtimer* this, z64_game_t* play) {
    if(this->actor.rot_init.z == 1)
        return z64_Flags_GetSwitch(play, this->switchFlag);
    return Flags_GetTempClear(play, this->actor.room_index);
}