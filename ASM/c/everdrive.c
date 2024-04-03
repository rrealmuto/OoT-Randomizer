#include <stdbool.h>
#include <mips.h>
#include <n64.h>

#include "everdrive.h"

#include "get_items.h"
#include "item_upgrades.h"
#include "z64.h"

#define REG_USB_CFG 0x0001
#define REG_EDID    0x0005
#define REG_USB_DAT 0x0100
#define REG_SYS_CFG 0x2000
#define REG_KEY     0x2001

#define REG_BASE 0xBF800000
#define REGS_PTR ((volatile uint32_t *)REG_BASE)

#define USB_LE_CFG 0x8000
#define USB_LE_CTR 0x4000

#define USB_CFG_RD  0x0400
#define USB_CFG_WR  0x0000
#define USB_CFG_ACT 0x0200

#define USB_STA_PWR 0x1000
#define USB_STA_RXF 0x0400
#define USB_STA_TXE 0x0800
#define USB_STA_ACT 0x0200

#define USB_CMD_RD      (USB_LE_CFG | USB_LE_CTR | USB_CFG_RD | USB_CFG_ACT)
#define USB_CMD_RD_NOP  (USB_LE_CFG | USB_LE_CTR | USB_CFG_RD)
#define USB_CMD_WR      (USB_LE_CFG | USB_LE_CTR | USB_CFG_WR | USB_CFG_ACT)
#define USB_CMD_WR_NOP  (USB_LE_CFG | USB_LE_CTR | USB_CFG_WR)

static int      cart_irqf;
static uint32_t cart_lat;
static uint32_t cart_pwd;
static uint16_t spi_cfg;

uint8_t everdrive_detection_state = ED64_DETECTION_UNKNOWN;

extern uint8_t EVERDRIVE_READ_BUF[16];

uint8_t EVERDRIVE_PROTOCOL_VERSION = 1;

uint8_t TIMEOUT_ERROR[16] = { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// set irq bit and return previous value
static inline int set_irqf(int irqf) {
  uint32_t sr;

  __asm__ ("mfc0    %[sr], $12;" : [sr] "=r"(sr));
  int old_irqf = sr & MIPS_STATUS_IE;

  sr = (sr & ~MIPS_STATUS_IE) | (irqf & MIPS_STATUS_IE);
  __asm__ ("mtc0    %[sr], $12;" :: [sr] "r"(sr));

  return old_irqf;
}

static inline void __pi_wait(void) {
    while (pi_regs.status & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)) {
        // busy loop
    }
}

static inline uint32_t __pi_read_raw(uint32_t dev_addr) {
    __pi_wait();
    return *(volatile uint32_t *)dev_addr;
}

static inline void __pi_write_raw(uint32_t dev_addr, uint32_t value) {
    __pi_wait();
    *(volatile uint32_t *)dev_addr = value;
}

static void pio_read(uint32_t dev_addr, uint32_t ram_addr, size_t size) {
    if (size == 0) {
        return;
    }

    uint32_t dev_s = dev_addr & ~0x3;
    uint32_t dev_e = (dev_addr + size + 0x3) & ~0x3;
    uint32_t dev_p = dev_s;

    uint32_t ram_s = ram_addr;
    uint32_t ram_e = ram_s + size;
    uint32_t ram_p = ram_addr - (dev_addr - dev_s);

    while (dev_p < dev_e) {
        uint32_t w = __pi_read_raw(dev_p);
        for (int i = 0; i < 4; i++) {
            if (ram_p >= ram_s && ram_p < ram_e) {
                *(uint8_t *)ram_p = w >> 24;
            }
            w <<= 8;
            ram_p++;
        }
        dev_p += 4;
    }
}

static void pio_write(uint32_t dev_addr, uint32_t ram_addr, size_t size)
{
  if (size == 0)
    return;

  uint32_t dev_s = dev_addr & ~0x3;
  uint32_t dev_e = (dev_addr + size + 0x3) & ~0x3;
  uint32_t dev_p = dev_s;

  uint32_t ram_s = ram_addr;
  uint32_t ram_e = ram_s + size;
  uint32_t ram_p = ram_addr - (dev_addr - dev_s);

  while (dev_p < dev_e) {
    uint32_t w = __pi_read_raw(dev_p);
    for (int i = 0; i < 4; i++) {
      uint8_t b;
      if (ram_p >= ram_s && ram_p < ram_e)
        b = *(uint8_t *)ram_p;
      else
        b = w >> 24;
      w = (w << 8) | b;
      ram_p++;
    }
    __pi_write_raw(dev_p, w);
    dev_p += 4;
  }
}

void pi_read_locked(uint32_t dev_addr, void *dst, size_t size) {
    pio_read(dev_addr, (uint32_t)dst, size);
}

void pi_write_locked(uint32_t dev_addr, void *dst, size_t size) {
    pio_write(dev_addr, (uint32_t)dst, size);
}

static inline uint32_t reg_rd(int reg) {
    return __pi_read_raw((uint32_t)&REGS_PTR[reg]);
}


static inline void reg_wr(int reg, uint32_t dat) {
    return __pi_write_raw((uint32_t)&REGS_PTR[reg], dat);
}

void cart_lock_safe() {
    z64_osPiGetAccess();
    cart_irqf = set_irqf(0);
    cart_lat = pi_regs.dom1_lat;
    cart_pwd = pi_regs.dom1_pwd;
}

void cart_unlock() {
    pi_regs.dom1_lat = cart_lat;
    pi_regs.dom1_pwd = cart_pwd;
    z64_osPiRelAccess();
    set_irqf(cart_irqf);
}

bool everdrive_detect() {
    if (everdrive_detection_state == ED64_DETECTION_UNKNOWN) {
        cart_lock_safe();
        reg_wr(REG_KEY, 0xAA55);
        switch (reg_rd(REG_EDID)) {
            case 0xED640008: // EverDrive 3.0
            case 0xED640013: // EverDrive X7
                // initialize USB
                reg_wr(REG_SYS_CFG, 0);
                cart_unlock();
                everdrive_detection_state = ED64_DETECTION_PRESENT;
                break;
            default: // EverDrive without USB support or no EverDrive
                reg_wr(REG_KEY, 0);
                cart_unlock();
                everdrive_detection_state = ED64_DETECTION_NOT_PRESENT;
                break;
        }
    }
    return everdrive_detection_state == ED64_DETECTION_PRESENT;
}

bool everdrive_read(uint8_t *buf) {
    cart_lock_safe();
    uint32_t len = 16; // for simplicity, the protocol is designed so each packet size is the EverDrive's minimum of 16 bytes
    uint32_t usb_cfg = reg_rd(REG_USB_CFG);
    if (!(usb_cfg & USB_STA_PWR)) {
        // cannot read or write (no USB device connected?)
        cart_unlock();
        return false;
    }
    if (usb_cfg & USB_STA_ACT) {
        // currently busy reading or writing
        cart_unlock();
        return false;
    }
    if (usb_cfg & USB_STA_RXF) {
        // no data waiting
        cart_unlock();
        return false;
    } else {
        // data waiting, start reading
        reg_wr(REG_USB_CFG, USB_CMD_RD | (512 - len));
        uint16_t timeout = 0;
        while (reg_rd(REG_USB_CFG) & USB_STA_ACT) {
            // still reading
            if (timeout++ == 8192) {
                // timed out
                for (int i = 0; i < 16; i++) {
                    buf[i] = TIMEOUT_ERROR[i];
                }
                return true;
            }
        }
        reg_wr(REG_USB_CFG, USB_CMD_RD_NOP);
        pi_read_locked((uint32_t)&REGS_PTR[REG_USB_DAT] + (512 - len), buf, len);
        cart_unlock();
        return true;
    }
}

bool everdrive_write(uint8_t *buf) {
    cart_lock_safe();
    uint32_t len = 16; // for simplicity, the protocol is designed so each packet size is the EverDrive's minimum of 16 bytes
    uint32_t usb_cfg = reg_rd(REG_USB_CFG);
    if (!(usb_cfg & USB_STA_PWR)) {
        // cannot read or write (no USB device connected?)
        cart_unlock();
        return false;
    }
    if (usb_cfg & USB_STA_ACT) {
        // currently busy reading or writing
        cart_unlock();
        return false;
    }
    if (usb_cfg & USB_STA_TXE) {
        // no data waiting
        cart_unlock();
        return false;
    } else {
        // data waiting, start writing
        reg_wr(REG_USB_CFG, USB_CMD_WR_NOP);
        pi_write_locked((uint32_t)&REGS_PTR[REG_USB_DAT] + (512 - len), buf, len);
        reg_wr(REG_USB_CFG, USB_CMD_WR | (512 - len));
        uint16_t timeout = 0;
        while (reg_rd(REG_USB_CFG) & USB_STA_ACT) {
            // still writing
            if (timeout++ == 8192) {
                // timed out
                return false;
            }
        }
        cart_unlock();
        return true;
    }
}

extern uint8_t CFG_RANDO_VERSION_MAJOR;
extern uint8_t CFG_RANDO_VERSION_MINOR;
extern uint8_t CFG_RANDO_VERSION_PATCH;
extern uint8_t CFG_RANDO_VERSION_BRANCH;
extern uint8_t CFG_RANDO_VERSION_SUPPLEMENTARY;
extern uint8_t PLAYER_ID;
extern uint8_t CFG_FILE_SELECT_HASH[5];
extern uint8_t MW_SEND_OWN_ITEMS;
extern uint8_t MW_PROGRESSIVE_ITEMS_ENABLE;
extern uint8_t PLAYER_NAMES[256][8];
extern mw_progressive_items_state_t MW_PROGRESSIVE_ITEMS_STATE[256];

uint8_t everdrive_in_game = 2;
char everdrive_file_name[0x08] = { 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF };
uint8_t everdrive_protocol_state = EVERDRIVE_PROTOCOL_STATE_INIT;

uint8_t EVERDRIVE_MESSAGE_PING[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t EVERDRIVE_MESSAGE_RESET[16] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t frames_since_last_ping = 0;

void everdrive_handshake() {
    if (EVERDRIVE_READ_BUF[0] == 'c' && EVERDRIVE_READ_BUF[1] == 'm' && EVERDRIVE_READ_BUF[2] == 'd' && EVERDRIVE_READ_BUF[3] == 't') {
        uint8_t reply[16] = {
            'O', 'o', 'T', 'R',
            EVERDRIVE_PROTOCOL_VERSION,
            CFG_RANDO_VERSION_MAJOR,
            CFG_RANDO_VERSION_MINOR,
            CFG_RANDO_VERSION_PATCH,
            CFG_RANDO_VERSION_BRANCH,
            CFG_RANDO_VERSION_SUPPLEMENTARY,
            PLAYER_ID,
            CFG_FILE_SELECT_HASH[0],
            CFG_FILE_SELECT_HASH[1],
            CFG_FILE_SELECT_HASH[2],
            CFG_FILE_SELECT_HASH[3],
            CFG_FILE_SELECT_HASH[4],
        };
        everdrive_write(reply);
        everdrive_protocol_state = EVERDRIVE_PROTOCOL_STATE_HANDSHAKE;
    } else {
        everdrive_write(EVERDRIVE_MESSAGE_RESET);
        everdrive_protocol_state = EVERDRIVE_PROTOCOL_STATE_INIT;
    }
}

void everdrive_update_in_game(bool in_game) {
    if (in_game) {
        uint8_t state_packet[16] = {
            0x02, // State: In Game
            z64_file.ammo[4], // internal item count, hi
            z64_file.ammo[5], // internal item count, lo
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //TODO send relevant parts of save data (which?)
        };
        everdrive_write(state_packet);
        everdrive_in_game = 1;
    } else {
        uint8_t state_packet[16] = {
            0x01, // State: File Select
            z64_file.file_name[0],
            z64_file.file_name[1],
            z64_file.file_name[2],
            z64_file.file_name[3],
            z64_file.file_name[4],
            z64_file.file_name[5],
            z64_file.file_name[6],
            z64_file.file_name[7],
            0, 0, 0, 0, 0, 0, 0,
        };
        everdrive_write(state_packet);
        everdrive_in_game = 0;
        for (int i = 0; i < 8; i++) {
            everdrive_file_name[i] = z64_file.file_name[i];
        }
    }
}

void everdrive_frame(bool in_game) {
    if (everdrive_detect()) {
        if (everdrive_protocol_state == EVERDRIVE_PROTOCOL_STATE_MW) {
            if (++frames_since_last_ping >= 5 * 20) {
                everdrive_write(EVERDRIVE_MESSAGE_PING);
                frames_since_last_ping = 0;
            }
            if (in_game) {
                if (
                    everdrive_in_game != 1
                    && z64_logo_state != 0x802C5880
                    && z64_logo_state != 0
                    && z64_file.game_mode == 0
                ) {
                    everdrive_update_in_game(in_game);
                }
            } else {
                bool filenames_match = true;
                for (int i = 0; i < 8; i++) {
                    if (z64_file.file_name[i] != everdrive_file_name[i]) {
                        filenames_match = false;
                        break;
                    }
                }
                if (everdrive_in_game != 0 || !filenames_match) {
                    everdrive_update_in_game(in_game);
                }
            }
        }
        if (everdrive_read(EVERDRIVE_READ_BUF)) {
            switch (everdrive_protocol_state) {
                case EVERDRIVE_PROTOCOL_STATE_INIT: {
                    everdrive_handshake();
                    break;
                }
                case EVERDRIVE_PROTOCOL_STATE_HANDSHAKE: {
                    if (EVERDRIVE_READ_BUF[0] == 'M' && EVERDRIVE_READ_BUF[1] == 'W') {
                        if (EVERDRIVE_READ_BUF[2] != EVERDRIVE_PROTOCOL_VERSION) {
                            everdrive_write(EVERDRIVE_MESSAGE_RESET);
                            everdrive_protocol_state = EVERDRIVE_PROTOCOL_STATE_INIT;
                        } else {
                            MW_SEND_OWN_ITEMS = EVERDRIVE_READ_BUF[3];
                            MW_PROGRESSIVE_ITEMS_ENABLE = EVERDRIVE_READ_BUF[4];
                            if (!in_game || (
                                z64_logo_state != 0x802C5880
                                && z64_logo_state != 0
                                && z64_file.game_mode == 0
                            )) {
                                everdrive_update_in_game(in_game);
                            } else {
                                everdrive_in_game = 2; // uninitialized; ensure state packet is sent
                            }
                            everdrive_protocol_state = EVERDRIVE_PROTOCOL_STATE_MW;
                        }
                    } else if (EVERDRIVE_READ_BUF[0] == 'c') {
                        everdrive_handshake();
                    } else {
                        everdrive_write(EVERDRIVE_MESSAGE_RESET);
                        everdrive_protocol_state = EVERDRIVE_PROTOCOL_STATE_INIT;
                    }
                    break;
                }
                case EVERDRIVE_PROTOCOL_STATE_MW: {
                    if (EVERDRIVE_READ_BUF[0] == 0x00) {
                        // ping
                    } else if (EVERDRIVE_READ_BUF[0] == 0x01) {
                        // player data
                        uint8_t player_id = EVERDRIVE_READ_BUF[1];
                        for (int i = 0; i < 8; i++) {
                            PLAYER_NAMES[player_id][i] = EVERDRIVE_READ_BUF[2 + i];
                        }
                        MW_PROGRESSIVE_ITEMS_STATE[player_id] = *((mw_progressive_items_state_t*) (&EVERDRIVE_READ_BUF[10]));
                    } else if (EVERDRIVE_READ_BUF[0] == 0x02) {
                        // get item
                        uint16_t incoming_item = EVERDRIVE_READ_BUF[1] << 8 | EVERDRIVE_READ_BUF[2];
                        override_t override = { 0 };
                        override.key.scene = 0xFF;
                        override.key.type = OVR_DELAYED;
                        override.key.flag = 0xFF;
                        override.value.base.player = incoming_item == 0xca ? (PLAYER_ID == 1 ? 2 : 1) : PLAYER_ID;
                        override.value.base.item_id = incoming_item;
                        push_pending_item(override);
                    } else if (EVERDRIVE_READ_BUF[0] == 'c') {
                        everdrive_handshake();
                    } else {
                        everdrive_write(EVERDRIVE_MESSAGE_RESET);
                        everdrive_protocol_state = EVERDRIVE_PROTOCOL_STATE_INIT;
                    }
                    break;
                }
            }
        }
    }
}
