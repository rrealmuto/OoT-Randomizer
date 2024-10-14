#include "z64.h"
#include "util.h"
#include "actor.h"

typedef void (EnFd_Update_Func)(z64_actor_t*, z64_game_t*);

extern EnFd_Update_Func OVL_EnFd_Update;

void EnFd_Update_Hook(z64_actor_t* this, z64_game_t* globalCtx) {
    EnFd_Update_Func* EnFd_Update = (EnFd_Update_Func*)(resolve_actor_overlay_addr(&OVL_EnFd_Update, this));
    EnFd_Update(this, globalCtx);
    Actor_UpdateBgCheckInfo(globalCtx, this, 100.0f, 100.0f, 100.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1);
}
