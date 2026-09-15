#include "z64.h"
#include "util.h"
#include "actor.h"
#include "en_fd.h"
#include "audio.h"

typedef void (EnFd_Update_Func)(z64_actor_t*, z64_game_t*);

extern EnFd_Update_Func OVL_EnFd_Update;
extern void Actor_SetPlayerKnockbackLargeNoDamage(z64_game_t* play, z64_actor_t* actor, float speed, int16_t rot, float yVelocity);

#define NA_SE_PL_BODY_HIT 0x83E

void EnFd_SetPlayerKnockbackLargeNoDamage(z64_game_t* play, z64_actor_t* actor, float speed, int16_t rot, float yVelocity) {
    // Check the collider actually collided with player to prevent flare dancer crashing into other collisions causing link to get knocked back
    // This happens a lot in the spirit temple iron knuckle rooms
    EnFd* this = (EnFd*)actor;
    z64_link_t* player = GET_PLAYER(play);
    if(this->collider.base.at == player) {
        Actor_PlaySfx(&player->common, NA_SE_PL_BODY_HIT);
        Actor_SetPlayerKnockbackLargeNoDamage(play, actor, speed, rot, yVelocity);
        return;
    }
    this->attackTimer = 0; // Didn't collide w/ player, so set attackTimer back to 0
}

void EnFd_Update_Hook(z64_actor_t* this, z64_game_t* globalCtx) {
    EnFd_Update_Func* EnFd_Update = (EnFd_Update_Func*)(resolve_actor_overlay_addr(&OVL_EnFd_Update, this));
    EnFd_Update(this, globalCtx);
    Actor_UpdateBgCheckInfo(globalCtx, this, 100.0f, 100.0f, 100.0f, UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1);
}
