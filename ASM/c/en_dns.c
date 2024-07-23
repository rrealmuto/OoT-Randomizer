#include "z64.h"
#include "util.h"
#include "en_dns.h"

const uint16_t* sStartingTextIds_Addr = (uint16_t*)0x80a75d2c;

void EnDns_SetTextId(z64_actor_t* thisx, float scale) {
    // Set the actor's text ID based on the message offset that was passed into the actor variable, and stored into the actor instance
    // If we were passed a message offset, use the new messages, otherwise use the existing messages
    EnDns* this = (EnDns*)thisx;
    if(this->messageOffset > 0) {
        if(z64_game.scene_index == 0x3E) {
            // In a grotto, message ID is 0xA100 + 0x10*grotto_id + messageOffset - 1
            uint8_t grotto_id = z64_file.grotto_id & 0x1F;
            uint16_t message_offset = (grotto_id * 0x10) + this->messageOffset - 1;
            this->actor.text_id = 0xA100 + message_offset;
        }
        else {
            // not in a grotto, message ID is just 0xA000 + messageOffset - 1
            this->actor.text_id = 0xA000 + this->messageOffset - 1;
        }
    }
    else {
        // Otherwise just use the original message ID
        // Resolve sStartingTextIds array from overlay
        uint16_t* sStartingTextIds = resolve_overlay_addr((void*)sStartingTextIds_Addr, thisx->actor_id);
        this->actor.text_id = sStartingTextIds[DNS_GET_TYPE(thisx)];
    }

    Actor_SetScale(thisx, scale);
}