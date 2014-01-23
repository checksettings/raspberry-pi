#ifndef MOTORAPI_H
#define MOTORAPI_H

#include "stdlib.h"

extern void motorInit(void);

extern uint32_t getMotorVersion(void);
extern uint32_t setMotorSpeed(uint32_t value);
extern uint32_t getMotorSpeed(void);


#endif	/* MOTORAPI_H */
