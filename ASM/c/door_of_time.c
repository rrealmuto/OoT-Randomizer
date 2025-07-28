#include "door_of_time.h"
#include "z64.h"

extern uint8_t DOT_CONDITION;

int32_t DemoKankyo_CutsceneFlags_Get_Hook(void* play, int16_t flag) {
    switch (DOT_CONDITION) {
        case 0: // open
            return 1;
        case 1: // sot
            return CutsceneFlags_Get(play, flag);
        case 2: // oot_sot
            return z64_file.items[Z64_SLOT_OCARINA] == 0x08 && CutsceneFlags_Get(play, flag);
        case 3: // stones
            return (z64_file.quest_items & 0x1C0000) == 0x1C0000;
        case 4: // stones_sot
            return (z64_file.quest_items & 0x1C0000) == 0x1C0000 && CutsceneFlags_Get(play, flag);
        case 5: // stones_oot_sot
            return (z64_file.quest_items & 0x1C0000) == 0x1C0000 && z64_file.items[Z64_SLOT_OCARINA] == 0x08 && CutsceneFlags_Get(play, flag);
    }
}

int32_t ShouldOpenDoorOfTime(void* play, int16_t flag) {
    switch (DOT_CONDITION) {
        case 0: // open
            return 1;
        case 1: // sot
            return 1;
        case 2: // oot_sot
            return z64_file.items[Z64_SLOT_OCARINA] == 0x08;
        case 3: // stones
            return (z64_file.quest_items & 0x1C0000) == 0x1C0000;
        case 4: // stones_sot
            return (z64_file.quest_items & 0x1C0000) == 0x1C0000;
        case 5: // stones_oot_sot
            return (z64_file.quest_items & 0x1C0000) == 0x1C0000 && z64_file.items[Z64_SLOT_OCARINA] == 0x08;
    }
}
