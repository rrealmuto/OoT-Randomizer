#include "z64.h"
#include "stdbool.h"

// Hack to add additional text control codes.
// If additional codes need to be read after the primary code, increment msgCtx->msgBufPos and index msgRaw
// To add new text to the output buffer, perform the following:
// Add character to msgCtx->msgBufDecoded[decodedBufPosVal]
// call Font_LoadChar(pFont, character, charTexIdx)
// Increment decodedBufPosVal (except for on the last character being added)
// Add 0x80 to charTexIdx
// When finished, copy decodedBufPosVal and charTexIdx back to their pointers
// And return true
bool Message_Decode_Additional_Control_Codes(MessageContext* msgCtx, uint8_t currChar, uint32_t* pDecodedBufPos, uint32_t* pCharTexIdx) {
    uint32_t decodedBufPosVal = *pDecodedBufPos;
    uint32_t charTexIdx = *pCharTexIdx;
    void* pFont = (void*)(((uint8_t*)(&z64_game)) + 0x2200); // Get a reference to the font. Our struct is obviously broken. Determined it is at 0x2200 + GlobalContext
    char* msgRaw = ((char*)pFont) + 0xDC88; // Get a reference to the start of the raw message. Index using msgCtx->msgBufPos. 
    
    /*
    Example new control code that replaces code 0xF0 w/ "ABC"
    if(currChar == 0xF0) { 
        msgCtx->msgBufDecoded[decodedBufPosVal++] = 0x41;
        Font_LoadChar(pFont, 0x41 - ' ', charTexIdx);
        charTexIdx += 0x80;
        msgCtx->msgBufDecoded[decodedBufPosVal++] = 0x42;
        Font_LoadChar(pFont, 0x42 - ' ', charTexIdx);
        charTexIdx += 0x80;
        msgCtx->msgBufDecoded[decodedBufPosVal] = 0x43; // Don't increment the last time because the main function increments it.
        Font_LoadChar(pFont, 0x43 - ' ', charTexIdx);
        charTexIdx += 0x80;
        *pDecodedBufPos = decodedBufPosVal;
        *pCharTexIdx = charTexIdx;
        //msgCtx->msgBufPos++;
        return true;
    }
    */ 
    return false;
}