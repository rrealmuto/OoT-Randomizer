#include "z64.h"
#include "gfx.h"
#include "models.h"

extern loaded_object_t object_hookshot_new;
void ArmsHook_Draw_SetupHack(z64_gfx_t* gfxCtx) {
    gSPSegment(gfxCtx->poly_opa.p++, 0x06, object_hookshot_new.buf);
    z64_Gfx_SetupDL_25Opa(gfxCtx);
}