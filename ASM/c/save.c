#include "z64.h"


#define SRAM_BASE 0x08000000
#define SRAM_NEWDATA_START 0x5160
#define SRAM_SIZE 0x8000
extern uint32_t collectible_override_flags[202];
extern uint32_t dropped_collectible_override_flags[808];

void Save_Write_Hook(uint32_t addr, void* dramAddr, size_t size, uint32_t direction)
{
    //Save the original data to ROM
    SsSram_ReadWrite(addr, dramAddr, size, direction);
    
    //Save some additional data to ROM
    uint16_t slot_offset = SRAM_NEWDATA_START + (z64_file.file_index)*(sizeof(collectible_override_flags) + sizeof(dropped_collectible_override_flags));
    SsSram_ReadWrite(SRAM_BASE + slot_offset, &collectible_override_flags, sizeof(collectible_override_flags), direction);
    SsSram_ReadWrite(SRAM_BASE + slot_offset + sizeof(collectible_override_flags), &dropped_collectible_override_flags, sizeof(dropped_collectible_override_flags), direction);
    //SsSram_ReadWrite(SRAM_BASE + SRAM_NEWDATA_START + ((z64_file.file_index) * sizeof(uint32_t)*808), &collectible_override_flags, sizeof(uint32_t) *808, 1);
    //SsSram_ReadWrite(SRAM_BASE + SRAM_NEWDATA_START + (2*sizeof(uint32_t) *808) + ((z64_file.file_index) * sizeof(uint32_t)*808), &dropped_collectible_override_flags, sizeof(uint32_t) *808, 1);
}

void Save_Open(char* sramBuffer)
{
    uint16_t slot_offset = SRAM_NEWDATA_START + (z64_file.file_index)*(sizeof(collectible_override_flags) + sizeof(dropped_collectible_override_flags));
    z64_memcopy(&collectible_override_flags, sramBuffer + slot_offset, sizeof(collectible_override_flags));
    z64_memcopy(&dropped_collectible_override_flags, sramBuffer + slot_offset + sizeof(collectible_override_flags), sizeof(dropped_collectible_override_flags));
    //z64_memcopy(&collectible_override_flags, sramBuffer + SRAM_NEWDATA_START + ((z64_file.file_index) * sizeof(uint32_t)*808), sizeof(uint32_t)*808);
    //z64_memcopy(&dropped_collectible_override_flags, sramBuffer + SRAM_NEWDATA_START + (2*sizeof(uint32_t) *808) + ((z64_file.file_index) * sizeof(uint32_t)*808), sizeof(uint32_t)*808);
}

void Save_Init_Write_Hook(uint32_t addr, void* dramAddr, size_t size, uint32_t direction)
{
    //zeroize the new collectible flags in the sram buffer (dramAddr)
    uint16_t slot_offset = SRAM_NEWDATA_START + (z64_file.file_index)*(sizeof(collectible_override_flags) + sizeof(dropped_collectible_override_flags));
    z64_bzero(dramAddr + slot_offset, sizeof(collectible_override_flags) + sizeof(dropped_collectible_override_flags));
    //z64_bzero(dramAddr + SRAM_NEWDATA_START + ((z64_file.file_index) * sizeof(uint32_t)*808), sizeof(uint32_t) * 808 );
    //z64_bzero(dramAddr + SRAM_NEWDATA_START + (2*sizeof(uint32_t) *808) + ((z64_file.file_index) * sizeof(uint32_t)*808),sizeof(uint32_t) *808);
    
    //write to sram
    SsSram_ReadWrite(SRAM_BASE, dramAddr, SRAM_SIZE, direction);
}