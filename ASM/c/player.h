#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "z64.h"
#include "util.h"

#define PLAYER_OVERLAY_FUNCTION(ret,name,args_with_types,args,addr) ret name args_with_types { ret (*name##_func)args_with_types = resolve_player_ovl_addr(addr); name##_func args;}

// 80388B60 - Player overlay usually loaded here
// 808301C0 - VRAM address of player overlay

#define z64_LinkInvincibility_addr              0x80835BD8
#define z64_LinkDamage_addr                     0x80835D08

PLAYER_OVERLAY_FUNCTION(void, z64_LinkInvincibility, (z64_link_t* link, uint8_t frame), (link,frame), z64_LinkInvincibility_addr)

PLAYER_OVERLAY_FUNCTION(void, z64_LinkDamage, (z64_game_t* ctxt, z64_link_t* link, uint8_t damage_type, float unk_00, uint32_t unk_01,uint16_t unk_02),(ctxt,link,damage_type,unk_00,unk_01,unk_02), z64_LinkDamage_addr)

#endif
