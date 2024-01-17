SECTIONS
{
    /* Allocatable and important sections */
    .text   : { *(.text*);   }
    .rodata : { *(.rodata*); }

    /*
     * Put the NOLOAD sections (bss, sbss, etc) into data to
     * force them to be in the ROM
     */
    .data   : { *(.data*);   *(.bss*);  *(COMMON);  }
    .sdata  : { *(.sdata*); *(.sbss*); *(.scommon); }

    /* Not allocatable sections that may be useful */
    .reginfo          : { *(.reginfo); }
    .pdr              : { *(.pdr); }
    .gnu.attributes   : { KEEP (*(.gnu.attributes)); }
    .mdebug           : { *(.mdebug); }
    .mdebug.abi32     : { *(.mdebug.abi32); }

    /* Useless allocatable sections */
    /DISCARD/ : {
        *(.MIPS.abiflags)
        *(.MIPS.options)
        *(.note.gnu.build-id)
        *(.got)
        *(.interp)
        *(.eh_frame)
        *(.reginfo)
    }
}
z64_ActorFind = 0x80025B0C;
KaleidoManager_GetRamAddr =  0x80099DBC;
DmaMgr_RequestSync = 0x80000df0;
SkelAnime_DrawFlexOpa = 0x80089d8c;
Actor_TalkOfferAccepted = 0x80022930;
Actor_OfferTalk = 0x80022a68;
Message_GetState = 0x800DD464;
Message_ShouldAdvance = 0x800d6110;
Message_ContinueTextbox = 0x800dce80;
Message_CloseTextbox = 0x800d6218;
Actor_OfferGetItem = 0x80022bd4;
z64_segments = 0x80120C38;
__osMallocInit = 0x800CDD90;
__osFree = 0x800CE2B4;
__osMalloc = 0x800CE060;
z64_LoadRoom_addr = 0x80080A3C;
z64_UnloadRoom = 0x80080C98;
