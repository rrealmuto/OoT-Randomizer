#include "z64.h"
#include "stdbool.h"

// Helper function for adding characters to the decoded message buffer
void Message_AddCharacter(MessageContext* msgCtx, void* pFont, uint32_t* pDecodedBufPos, uint32_t* pCharTexIdx, uint8_t charToAdd) {
    uint32_t decodedBufPosVal = *pDecodedBufPos;
    uint32_t charTexIdx = *pCharTexIdx;
    msgCtx->msgBufDecoded[decodedBufPosVal++] = charToAdd; // Add the character to the output buffer, increment the output position
    if(charToAdd != ' ') // Only load the character texture if it's not a space.
    {
        Font_LoadChar(pFont, charToAdd - ' ', charTexIdx); // Load the character texture
        charTexIdx += 0x80; // Increment the texture pointer
    }

    // Copy our locals back to their pointers
    *pDecodedBufPos = decodedBufPosVal;
    *pCharTexIdx = charTexIdx;
}

// Helper function for adding integer numbers to the decoded message buffer
void Message_AddInteger(MessageContext * msgCtx, void* pFont, uint32_t* pDecodedBufPos, uint32_t* pCharTexIdx, uint32_t numToAdd) {
    uint8_t digits[10];
    uint8_t i = 0;
    
    // Extract each digit. They are added, in reverse order, to digits[]
    do {
        digits[i] = numToAdd % 10;
        numToAdd = numToAdd / 10;
        i++;
    }

    // Loop through each digit in digits[] and add the character to the decoded buffer.
    while ( numToAdd > 0 );
    for(uint8_t c = i; c > 0; c--) {
        Message_AddCharacter(msgCtx, pFont, pDecodedBufPos, pCharTexIdx, '0' + digits[c - 1]);
    }
}

// Helper function for adding simple strings to the decoded message buffer. Does not support additional control codes.
void Message_AddString(MessageContext * msgCtx, void* pFont, uint32_t* pDecodedBufPos, uint32_t* pCharTexIdx, char* stringToAdd)
{
    while(*stringToAdd != 0)
    {
        Message_AddCharacter(msgCtx, pFont, pDecodedBufPos, pCharTexIdx, *stringToAdd);
        stringToAdd++;
    }
}

// Hack to add additional text control codes.
// If additional codes need to be read after the primary code, increment msgCtx->msgBufPos and index msgRaw
// To add new text to the output buffer, perform the following:
//      Call one of the above functions to add the text.
//      Subtract 1 from *pDecodedBufPos
//      Return true
bool Message_Decode_Additional_Control_Codes(MessageContext* msgCtx, uint8_t currChar, uint32_t* pDecodedBufPos, uint32_t* pCharTexIdx) {
    uint32_t decodedBufPosVal = *pDecodedBufPos;
    uint32_t charTexIdx = *pCharTexIdx;
    void* pFont = (void*)(((uint8_t*)(&z64_game)) + 0x2200); // Get a reference to the font. Our struct is obviously broken. Determined it is at 0x2200 + GlobalContext
    char* msgRaw = ((char*)pFont) + 0xDC88; // Get a reference to the start of the raw message. Index using msgCtx->msgBufPos. 
    
    /*
    Example new control code that replaces code 0xF0 w/ a number
    if(currChar == 0xF0) { 
        uint32_t val = 20;
        Message_AddInteger(msgCtx, pFont, pDecodedBufPos, pCharTexIdx, val);
        (*pDecodedBufPos)--;
        return true;
    }
    */ 
    return false;
}