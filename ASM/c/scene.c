#include "z64.h"

int8_t curr_scene_setup;

void get_current_scene_setup_number()
{
    z64_scene_command* cmd = z64_game.scene_segment;
    
    if(z64_file.scene_setup_index == 0)
    {
        curr_scene_setup = 0;
        return;
    }
    
    if(z64_file.scene_setup_index > 3)
    {
        curr_scene_setup = -1;
        return;
    }

    while(1)
    {
        if(cmd->code == 0x14)
            break;

        if(cmd->code == 0x18) //Alternate header command
        {
            //Get segment number from alternate header command
            uint8_t segment = (cmd->data2 << 4) >> 28;
            //Get segment offset from alternate header command
            uint32_t segment_offset = (cmd->data2 & 0x00FFFFFF);
            void** alternate_header = (void**)(z64_game.scene_segment + segment_offset);
            uint8_t i = z64_file.scene_setup_index;
            while(i > 0)
            {
                if((alternate_header)[i-1] != NULL)
                {
                    curr_scene_setup = i;
                    return;
                }
                i--;
            }
            
        }
        cmd++;
    }
    curr_scene_setup = 0;
}