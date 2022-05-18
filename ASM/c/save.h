#include "z64.h"

typedef struct
{
    uint8_t silver_rupee_counts[0x16];
} extended_savecontext_static_t;


typedef union {
    uint32_t all;
    struct {
        uint16_t offset;
        uint8_t type;
        uint8_t value;
    };
} extended_initial_save_entry;


extern extended_initial_save_entry* EXTENDED_INITIAL_SAVE_DATA;
extern extended_savecontext_static_t extended_savectx;