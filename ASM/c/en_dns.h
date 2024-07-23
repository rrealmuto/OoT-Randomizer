#include "z64.h"
#ifndef __EN_DNS_H__
# define __EN_DNS_H__

#define BUSINESS_SCRUB_LIMB_MAX 18

#define DNS_GET_TYPE(thisx) ((thisx)->variable)

struct EnDns;

typedef void (*EnDnsActionFunc)(struct EnDns*, z64_game_t* globalCtx);
typedef uint32_t (*EnDnsCanBuyFunc)(struct EnDns*);
typedef void (*EnDnsPaymentFunc)(struct EnDns*);

typedef struct {
    /* 0x00 */ int16_t itemPrice;
    /* 0x02 */ uint16_t itemAmount;
    /* 0x04 */ int32_t getItemId;
    /* 0x08 */ EnDnsCanBuyFunc canBuy;
    /* 0x0C */ EnDnsPaymentFunc payment;
} DnsItemEntry; // size = 0x10

typedef struct EnDns {
    /* 0x0000 */ z64_actor_t actor;
    /* 0x013C */ uint8_t skelAnime[0x44];
    /* 0x0180 */ z64_xyz_t jointTable[BUSINESS_SCRUB_LIMB_MAX];
    /* 0x01EC */ z64_xyz_t morphTable[BUSINESS_SCRUB_LIMB_MAX];
    /* 0x0258 */ EnDnsActionFunc actionFunc;
    /* 0x025C */ ColliderCylinder collider;
    /* 0x02A8 */ int16_t dustTimer;
    /* 0x02AA */ uint8_t animIndex; // set but not read
    /* 0x02AB */ uint8_t bumpOn;
    /* 0x02AC */ uint8_t standOnGround;
    /* 0x02AD */ uint8_t dropCollectible;
    /* 0x02B0 */ DnsItemEntry* dnsItemEntry;
    /* 0x02B4 */ float yInitPos;
    // Extended info
    /* 0x02B8 */ uint8_t messageOffset;
} EnDns; // extended_size = 0x02C8

#endif
