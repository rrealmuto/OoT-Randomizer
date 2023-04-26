#include "z64.h"
#include "stdbool.h"
#include "save.h"

// Helper function for adding characters to the decoded message buffer
void Message_AddCharacter(MessageContext *msgCtx, void *pFont, uint32_t *pDecodedBufPos, uint32_t *pCharTexIdx, uint8_t charToAdd) {
    uint32_t decodedBufPosVal = *pDecodedBufPos;
    uint32_t charTexIdx = *pCharTexIdx;
    msgCtx->msgBufDecoded[decodedBufPosVal++] = charToAdd; // Add the character to the output buffer, increment the output position
    if (charToAdd != ' ') { // Only load the character texture if it's not a space.
        Font_LoadChar(pFont, charToAdd - ' ', charTexIdx); // Load the character texture
        charTexIdx += 0x80; // Increment the texture pointer
    }

    // Copy our locals back to their pointers
    *pDecodedBufPos = decodedBufPosVal;
    *pCharTexIdx = charTexIdx;
}

// Helper function for adding integer numbers to the decoded message buffer
void Message_AddInteger(MessageContext *msgCtx, void *pFont, uint32_t *pDecodedBufPos, uint32_t *pCharTexIdx, uint32_t numToAdd) {
    uint8_t digits[10];
    uint8_t i = 0;
    // Extract each digit. They are added, in reverse order, to digits[]
    do {
        digits[i] = numToAdd % 10;
        numToAdd = numToAdd / 10;
        i++;
    }
    // Loop through each digit in digits[] and add the character to the decoded buffer.
    while (numToAdd > 0);

    for (uint8_t c = i; c > 0; c--) {
        Message_AddCharacter(msgCtx, pFont, pDecodedBufPos, pCharTexIdx, '0' + digits[c - 1]);
    }
}

// Helper function for adding simple strings to the decoded message buffer. Does not support additional control codes.
void Message_AddString(MessageContext *msgCtx, void *pFont, uint32_t *pDecodedBufPos, uint32_t *pCharTexIdx, char *stringToAdd) {
    while (*stringToAdd != 0) {
        Message_AddCharacter(msgCtx, pFont, pDecodedBufPos, pCharTexIdx, *stringToAdd);
        stringToAdd++;
    }
}

// Hack to add additional text control codes.
// If additional codes need to be read after the primary code, increment msgCtx->msgBufPos and index msgRaw
// To add a new control code:
//      Compare currChar to the control code.
//          Add text to the output buffer by performing the following:
//          Call one of the above functions to add the text.
//          Subtract 1 from *pDecodedBufPos
//          Return true
bool Message_Decode_Additional_Control_Codes(uint8_t currChar, uint32_t *pDecodedBufPos, uint32_t *pCharTexIdx) {
    MessageContext *msgCtx = &(z64_game.msgContext);
    Font *pFont = &(msgCtx->font); // Get a reference to the font.
    char *msgRaw = (char *) &(pFont->msgBuf); // Get a reference to the start of the raw message. Index using msgCtx->msgBufPos.

    switch (currChar) {
        case 0xF0: {
            // Silver rupee puzzle control code
            // Get the next character which tells us which puzzle it's for
            uint8_t puzzle = msgRaw[++(msgCtx->msgBufPos)];
            uint8_t count = extended_savectx.silver_rupee_counts[puzzle];
            Message_AddInteger(msgCtx, pFont, pDecodedBufPos, pCharTexIdx, count);
            (*pDecodedBufPos)--;
            return true;
        }
        case 0xF1: {
            // Small key count
            // Get the next character which tells us which dungeon it's for
            uint8_t dungeon = msgRaw[++(msgCtx->msgBufPos)];
            uint8_t count = z64_file.scene_flags[dungeon].unk_00_ >> 0x10;
            Message_AddInteger(msgCtx, pFont, pDecodedBufPos, pCharTexIdx, count);
            (*pDecodedBufPos)--;
            return true;
        }
        default: {
            return false;
        }
    }
}
