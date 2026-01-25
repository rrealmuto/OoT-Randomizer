#include "en_rd.h"
#include "z64.h"

int32_t EnRd_FlagsGetSwitch_Hack(z64_game_t* play, int32_t flag) {
    if (flag == 0x7F) {
        return 1;
    }
    return z64_Flags_GetSwitch(play, flag);
}