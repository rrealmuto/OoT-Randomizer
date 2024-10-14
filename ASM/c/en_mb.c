#include "z64.h"
#include "util.h"
#include "en_mb.h"

typedef void (*EnMb_Func)(z64_actor_t* this, z64_game_t* globalCtx);\
typedef void (*EnMb_SetupFunc)(EnMb* this);
extern EnMb_Func OVL_EnMb_Init;
extern EnMbActionFunc OVL_EnMb_ClubWaitPlayerNear;
extern EnMb_SetupFunc OVL_EnMb_SetupClubWaitPlayerNear;

void EnMb_ClubWaitPlayerNearAndRotate(EnMb* this, z64_game_t* globalCtx);

void EnMb_Init_Hooked(z64_actor_t* thisx, z64_game_t* globalCtx) {
    // Check for additional moblin type
    EnMb* this = (EnMb*)thisx;
    bool is_new_moblin = false;
    this->path = 0;
    if(this->actor.variable == 0x02) {
        is_new_moblin = true;
        this->actor.variable = 0; // Reset to the original hammer moblin type to utilize its init case
        this->path = 1;
    }

    // Call the original init function
    EnMb_Func EnMb_Init = resolve_actor_overlay_addr(&OVL_EnMb_Init, this);
    EnMb_Init(this, globalCtx);

    if(is_new_moblin) {
        // Reset some of the stuff set by the default init case
        this->actor.pos_world.z = this->actor.pos_init.z;
        // Set up the new action func
        this->actionFunc = EnMb_ClubWaitPlayerNearAndRotate;
    }
}

void EnMb_SetupClubWaitPlayerNear_Hooked(EnMb* this) {
    // Call the original function
    EnMb_SetupFunc EnMb_SetupClubWaitPlayerNear = resolve_actor_overlay_addr(&OVL_EnMb_SetupClubWaitPlayerNear, &this->actor);
    EnMb_SetupClubWaitPlayerNear(this);

    // Check if we're the rotating club kind
    if(this->path == 1) {
        this->actionFunc = EnMb_ClubWaitPlayerNearAndRotate;
    }
}

void EnMb_ClubWaitPlayerNearAndRotate(EnMb* this, z64_game_t* globalCtx) {
    // Rotate towards player
    int16_t yawDiff = this->actor.yawTowardsPlayer - this->actor.rot_world.y;
    int16_t yaw = 0;
    if (yawDiff > 0x1000) {
        yaw = 0x800;
    }
    else if (yawDiff < -0x1000) {
        yaw = -0x800;
    }
    this->actor.rot_world.y += yaw;
    this->actor.rot_2.y += yaw;
    EnMbActionFunc EnMb_ClubWaitPlayerNear = resolve_actor_overlay_addr(&OVL_EnMb_ClubWaitPlayerNear, &this->actor);
    EnMb_ClubWaitPlayerNear(this, globalCtx);
}
