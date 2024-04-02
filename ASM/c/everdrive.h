#ifndef EVERDRIVE_H
#define EVERDRIVE_H

#include <stdbool.h>

#define ED64_DETECTION_UNKNOWN 0
#define ED64_DETECTION_PRESENT 1
#define ED64_DETECTION_NOT_PRESENT 2

#define EVERDRIVE_PROTOCOL_STATE_INIT      0x00
#define EVERDRIVE_PROTOCOL_STATE_HANDSHAKE 0x01
#define EVERDRIVE_PROTOCOL_STATE_MW        0x02

bool everdrive_detect();
bool everdrive_read(uint8_t *buf);
bool everdrive_write(uint8_t *buf);
void everdrive_frame();

#endif
