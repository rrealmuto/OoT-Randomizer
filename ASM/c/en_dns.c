#include "z64.h"
#include "util.h"
#include "en_dns.h"
#include "actor.h"

extern uint16_t* OVL_EN_DNS_sStartingTextIds; // Vanilla text ID array
extern DnsItemEntry** OVL_EN_DNS_sItemEntries; // Vanilla item entries array
extern uint8_t CFG_SCRUB_SHUFFLE;

typedef struct {
    xflag_t flag;
    uint16_t message_id;
    DnsItemEntry entry;
} DnsItemEntryExtended;

DnsItemEntryExtended SHOP_SCRUB_DATA[64];
extern xflag_t* spawn_actor_with_flag;

// Rewrite these functions so we don't need to resolve them from the overlay
uint32_t EnDns_CanBuyPrice_New(EnDns* this) {
    if (z64_file.rupees < this->dnsItemEntry->itemPrice) {
        return DNS_CANBUY_RESULT_NEED_RUPEES;
    }
    return DNS_CANBUY_RESULT_SUCCESS;
}

void EnDns_PayPrice_New(EnDns* this) {
    Rupees_ChangeBy(-this->dnsItemEntry->itemPrice);
}

DnsItemEntryExtended* EnDns_GetItemEntryExtended(EnDns* this) {
    // Get the xflag. Because we're in the init function it hasn't actually been stored in the actor yet
    xflag_t xflag = *spawn_actor_with_flag;
    
    // Use our new table to resolve the DnsItemEntry
    for (int i = 0; i < 64; i++) {
        if (SHOP_SCRUB_DATA[i].flag.all == 0) {
            return NULL;
        }
        else if ((SHOP_SCRUB_DATA[i].flag.scene == xflag.scene) && (SHOP_SCRUB_DATA[i].flag.all == xflag.all)) {
            return &(SHOP_SCRUB_DATA[i]);
        }
    }
    return NULL;
}

// Hack at the end of EnDns_Init
void EnDns_Init_End(EnDns* this) {
    DnsItemEntryExtended* entry = EnDns_GetItemEntryExtended(this);
    this->actor.text_id = entry->message_id;
    this->dnsItemEntry = &entry->entry;
    if (!CFG_SCRUB_SHUFFLE) {
        DnsItemEntry** sItemEntries = resolve_overlay_addr((void*)&OVL_EN_DNS_sItemEntries, this->actor.actor_id);
        this->dnsItemEntry->getItemId = sItemEntries[DNS_GET_TYPE(&this->actor)]->getItemId;
    }
}

/*
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
        uint16_t* sStartingTextIds = resolve_overlay_addr((void*)&OVL_EN_DNS_sStartingTextIds, thisx->actor_id);
        this->actor.text_id = sStartingTextIds[DNS_GET_TYPE(thisx)];
    }

    Actor_SetScale(thisx, scale); // Replaced code
}
*/