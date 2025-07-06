#include "z64.h"

extern uint8_t CFG_GLOOM;
int32_t Health_ChangeBy(z64_game_t* play, int16_t amount);

int32_t Health_ChangeBy_Hooked(z64_game_t* play, int16_t amount) {
    // Gloom setting decreases players max health every time they are hit
    if (CFG_GLOOM && amount < 0) {
        z64_file.energy_capacity -= 0x10;
        if(z64_file.energy_capacity < 0x0) {
            z64_file.energy_capacity = 0x0;
        }
    }

    return Health_ChangeBy(play, amount);
}