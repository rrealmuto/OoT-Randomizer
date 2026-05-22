#include "player.h"

PLAYER_OVERLAY_FUNCTION(void, z64_LinkInvincibility, (z64_link_t* link, uint8_t frame), (link,frame), OVL_LinkInvincibility_addr)
PLAYER_OVERLAY_FUNCTION(void, z64_LinkDamage, (z64_game_t* ctxt, z64_link_t* link, uint8_t damage_type, float unk_00, uint32_t unk_01,uint16_t unk_02),(ctxt,link,damage_type,unk_00,unk_01,unk_02), OVL_LinkDamage_addr)
PLAYER_OVERLAY_FUNCTION(void, Player_UseItem, (z64_game_t *game, z64_link_t *link, uint8_t item, uint8_t button), (game,link,item,button), OVL_Player_UseItem)
