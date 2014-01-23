#ifndef BLCTRL_H
#define BLCTRL_H

#define BLCTRL_DEFAULT_ADDRESS        0x52  // BLCTRL Default address
#define BLCTRL_WAITING                0xF0000

uint32_t BLCTRLsetMotorSpeed(uint32_t value);

#endif /* BLCTRL_H */
