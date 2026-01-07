#include "z64.h"
#include "door_shutter.h"
#include "util.h"

extern DoorShutterActionFunc OVL_DoorShutter_BarAndWaitSwitchFlag;
extern int8_t gPrevRoom;

// New Action Function for DoorShutter for Dodongo's Cavern Lizalfos fight with enemizer
// Need to check the temp clear flag and then set just the switch flag to open the door
// For any other room just use the default behavior
void DoorShutter_BarAndWaitSwitchFlag_Override(DoorShutter* this, z64_game_t* globalCtx) {
    DoorShutterActionFunc DoorShutter_BarAndWaitSwitchFlag = resolve_actor_overlay_addr(&OVL_DoorShutter_BarAndWaitSwitchFlag, &this->dyna.actor);

    if(globalCtx->scene_index == 1 && globalCtx->room_ctx.curRoom.num == 3) {
        if(Flags_GetTempClear(globalCtx, globalCtx->room_ctx.curRoom.num)) {
            // Figure out if we should set this switch flag based on the previous room
            int16_t flag = this->dyna.actor.variable & 0x3F;
            if (flag == 0x05) { // Lower Room
                if (gPrevRoom == 4 || gPrevRoom == 1) {
                    z64_Flags_SetSwitch(globalCtx, this->dyna.actor.variable & 0x3F);
                }
            }
            else if (flag == 0x06) { // Upper Room
                if (gPrevRoom == 10 || gPrevRoom == 12) {
                    z64_Flags_SetSwitch(globalCtx, this->dyna.actor.variable & 0x3F);
                }
            }

        }
    }
    DoorShutter_BarAndWaitSwitchFlag(this, globalCtx);
}
