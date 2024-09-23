#ifndef KALEIDO_ITEM_H
#define KALEIDO_ITEM_H

#include "z64.h"
#include "kaleido.h"

#define PAUSE_ITEM_NONE 999

#define PAUSE_CURSOR_TRANSITIONING_FROM_PAGE 9
#define PAUSE_CURSOR_PAGE_LEFT 10
#define PAUSE_CURSOR_PAGE_RIGHT 11

extern uint8_t OVL_gAmmoItems[16];
extern void* z64_EquippedItemOutlineTex[0x400];

/* Reimplemented */
void KaleidoScope_DrawItemSelect(z64_game_t* play);

/* Referenced */
typedef void (KaleidoScope_DrawAmmoCount_func)(z64_pause_ctxt_t* pause_ctxt, z64_gfx_t* gfx_ctxt, int16_t item);
typedef void (KaleidoScope_SetCursorVtx_func)(z64_pause_ctxt_t* pause_ctxt, uint16_t index, Vtx* vtx);
typedef Gfx* (KaleidoScope_QuadTextureIA8_func)(Gfx* gfx_ctx, void* texture, int16_t width, int16_t height, uint16_t point);
typedef void (KaleidoScope_MoveCursorToSpecialPos_func)(z64_game_t* play, uint16_t special_position);
typedef void (KaleidoScope_DrawQuadTextureRGBA32_func)(z64_gfx_t* gfx_ctxt, void* texture, uint16_t width, uint16_t height, uint16_t point);
typedef void (KaleidoScope_DrawCursor_func)(z64_game_t* play, int16_t page_index);

extern KaleidoScope_DrawAmmoCount_func OVL_KaleidoScope_DrawAmmoCount;
extern KaleidoScope_SetCursorVtx_func OVL_KaleidoScope_SetCursorVtx;
extern KaleidoScope_QuadTextureIA8_func OVL_KaleidoScope_QuadTextureIA8;
extern KaleidoScope_MoveCursorToSpecialPos_func OVL_KaleidoScope_MoveCursorToSpecialPos;
extern KaleidoScope_DrawQuadTextureRGBA32_func OVL_KaleidoScope_DrawQuadTextureRGBA32;
extern KaleidoScope_DrawCursor_func OVL_KaleidoScope_DrawCursor;

#endif //KALEIDO_ITEM_H
