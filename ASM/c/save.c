#include "z64.h"


#define SRAM_BASE 0x08000000
#define SRAM_NEWDATA_START 0x65B0
#define SRAM_SIZE 0x8000
extern uint32_t collectible_override_flags[202];
extern uint32_t dropped_collectible_override_flags[202];

void Save_Write_Hook(uint32_t addr, void* dramAddr, size_t size, uint32_t direction)
{
    //Save the original data to ROM
    SsSram_ReadWrite(addr, dramAddr, size, direction);
    
    //Save some additional data to ROM
    SsSram_ReadWrite(SRAM_BASE + SRAM_NEWDATA_START + ((z64_file.file_index) * sizeof(uint32_t)*202), &collectible_override_flags, sizeof(uint32_t) *202, 1);
    SsSram_ReadWrite(SRAM_BASE + SRAM_NEWDATA_START + (2*sizeof(uint32_t) *202) + ((z64_file.file_index) * sizeof(uint32_t)*202), &dropped_collectible_override_flags, sizeof(uint32_t) *202, 1);
}

void Save_Open(char* sramBuffer)
{
    z64_memcopy(&collectible_override_flags, sramBuffer + SRAM_NEWDATA_START + ((z64_file.file_index) * sizeof(uint32_t)*202), sizeof(uint32_t)*202);
    z64_memcopy(&dropped_collectible_override_flags, sramBuffer + SRAM_NEWDATA_START + (2*sizeof(uint32_t) *202) + ((z64_file.file_index) * sizeof(uint32_t)*202), sizeof(uint32_t)*202);
}

void Save_Init_Write_Hook(uint32_t addr, void* dramAddr, size_t size, uint32_t direction)
{
    //zeroize the new collectible flags in the sram buffer (dramAddr)
    z64_bzero(dramAddr + SRAM_NEWDATA_START + ((z64_file.file_index) * sizeof(uint32_t)*202), sizeof(uint32_t) * 202 );
    z64_bzero(dramAddr + SRAM_NEWDATA_START + (2*sizeof(uint32_t) *202) + ((z64_file.file_index) * sizeof(uint32_t)*202),sizeof(uint32_t) *202);
    
    //write to sram
    SsSram_ReadWrite(SRAM_BASE, dramAddr, SRAM_SIZE, direction);
}