#include "z64.h"
#include "en_ik.h"
#include "util.h"
#include "actor.h"

typedef void(EnIk_Setup_Func)(EnIk*);

extern EnIk_Setup_Func OVL_EnIk_SetupStandUp;
extern EnIk_Setup_Func OVL_EnIk_SetupWalkOrRun;

void EnIk_InitSetup_Hack(EnIk* this){ 
    EnIk_Setup_Func* EnIk_SetupStandUp = resolve_overlay_addr(OVL_EnIk_SetupStandUp, this->actor.actor_id);
    EnIk_Setup_Func* EnIk_SetupWalkOrRun = resolve_overlay_addr(OVL_EnIk_SetupWalkOrRun, this->actor.actor_id);

    if (this->spawnMode) {
        this->actor.flags |= ACTOR_FLAG_0 | ACTOR_FLAG_2;
        EnIk_SetupWalkOrRun(this);
    }
    else {
        EnIk_SetupStandUp(this);
    }
}