#ifndef MAGNETOMETERAPI_H
#define MAGNETOMETERAPI_H

#include "stdlib.h"

extern void magnetometerInit(void);

extern uint32_t magGetVersion(void);
extern uint32_t magGetBearing(void);


#endif	/* MAGNETOMETERAPI_H */
