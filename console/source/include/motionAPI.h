#ifndef MOTIONAPI_H
#define MOTIONAPI_H

#include "stdlib.h"

extern void motionInit(void);

extern uint32_t getMotVersion(void);
extern uint32_t getMotion(uint16_t* motions);
extern uint32_t getMotInit(void);

#endif	/* MOTIONAPI_H */
