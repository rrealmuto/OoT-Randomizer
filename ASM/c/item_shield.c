#include "z64.h"

extern void z64_Gfx_SetupDL_25Opa(z64_gfx_t* gfxCtx);
extern Gfx gCullBackDList[];

// Replaced setup called by ItemShield_Draw
void ItemShield_Draw_Setup(z64_gfx_t* gfxCtx) {
    // Set up segment 0x0C 
    gSPSegment(gfxCtx->poly_opa.p++, 0x0C, gCullBackDList);
    // Call the original function
    z64_Gfx_SetupDL_25Opa(gfxCtx);
}