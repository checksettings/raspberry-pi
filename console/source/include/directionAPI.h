#ifndef DIRECTIONAPI_H
#define DIRECTIONAPI_H

#include "stdlib.h"

extern void directionInit(void);

extern uint32_t getDirVersion(void);
extern uint32_t getDirection(uint16_t* directions);
extern uint32_t getTemperature(void);
extern uint32_t getDirInit(void);

#endif	/* DIRECTIONAPI_H */
