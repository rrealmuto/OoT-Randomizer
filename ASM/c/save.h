#include "z64.h"

typedef struct
{
    uint8_t silver_rupee_counts[0x16];
} extended_savecontext_static_t;

extern extended_savecontext_static_t extended_savectx;