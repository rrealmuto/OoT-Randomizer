#include "z64.h"
#include "actor.h"
typedef void(*BossGanon_Func)(z64_actor_t*, z64_game_t*);

void BossGanon_StartTextbox_Hack(z64_game_t* globalCtx, uint16_t messageId, z64_actor_t* actor) {
    if(z64_file.dogParams) {
        messageId = 0x70C8;
        BossGanon_Func BossGanon_SetupDeathCutscene = Actor_ResolveOverlayAddr(actor, 0x809f44bc);
        BossGanon_SetupDeathCutscene(actor, globalCtx);
    }
    z64_DisplayTextbox(globalCtx, messageId, NULL);
}

/*void BossGanon_UpdateDamage_Hook(z64_actor_t* this, z64_game_t* globalCtx) {
    BossGanon_Func BossGanon_UpdateDamage = Actor_ResolveOverlayAddr(this, 0x809f86d0);
    if(z64_file.dog_params) {
        BossGanon_Func BossGanon_SetupDeathCutscene = Actor_ResolveOverlayAddr(this, 0x809f44bc);
        BossGanon_SetupDeathCutscene(this, globalCtx);
        return;
    }
    BossGanon_UpdateDamage(this, globalCtx);
}*/
