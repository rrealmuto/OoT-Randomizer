#include "z64.h"
#include "util.h"
#include "en_tp.h"
#include "matrix.h"
#include "actor.h"

void OVL_EnTp_Update(z64_actor_t* this, z64_game_t* play);
typedef void (*EnTp_Update_Func)(z64_actor_t*, z64_game_t*);
