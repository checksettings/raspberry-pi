#include "magnetometerAPI.h"
#include "sensorconfig.h"

#include "stdio.h"

#include "cmps10.h"

uint32_t (*magGetVersionPtr)();
uint32_t (*magGetBearingPtr)();

void magnetometerInit(void)
{
  printf("magnetometerAPI\n");
  magGetVersionPtr = 0;
  magGetBearingPtr = 0;
  #ifdef CMPS10
    printf("- CMPS10\n");
    magGetVersionPtr = &CMPS10getVersion;
    magGetBearingPtr = &CMPS10getBearing;
  #endif
}

uint32_t magGetBearing(void)
{
  if (magGetBearingPtr != 0)
    return (*magGetBearingPtr)();
  return -1U;
}

uint32_t magGetVersion(void)
{
  if (magGetVersionPtr != 0)
    return (*magGetVersionPtr)();
  return -1U;
}
