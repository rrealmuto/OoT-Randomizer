#include "z64.h"


#define SRAM_END 0x8000

extern uint32_t collectible_override_flags[202];

void Save_Write_Hook(uint32_t addr, void* dramAddr, size_t size, uint32_t direction)
{
    //Save the original data to ROM
    SsSram_ReadWrite(addr, dramAddr, size, direction);
    
    //Save some additional data to ROM
    SsSram_ReadWrite(addr + SRAM_END - (sizeof(uint32_t)*202), &collectible_override_flags, sizeof(uint32_t) *202, 1);
}

void Save_Read_Hook(uint32_t addr, void* dramAdd, size_t size, uint32_t direction)
{
    SsSram_ReadWrite(addr + SRAM_END - (sizeof(uint32_t*202), &collectible_overide_flags, sizeof(uint32_t) * 202, 0))
}