#include "distanceAPI.h"
#include "sensorconfig.h"

#include "stdio.h"

#include "srf08.h"

uint32_t (*getDistVersionPtr)();
uint32_t (*getDistancePtr)();

void distanceInit(void)
{
  printf("distanceInit\n");
  getDistVersionPtr = 0;
  getDistancePtr = 0;
  #ifdef SRF08
    printf("- SRF08\n");
    getDistVersionPtr = &SRF08getVersion;
    getDistancePtr = &SRF08getDistance;
  #endif
}

uint32_t getDistance(void)
{
  if (getDistancePtr != 0)
    return (*getDistancePtr)();
  return -1U;
}

uint32_t getDistVersion(void)
{
  if (getDistVersionPtr != 0)
    return (*getDistVersionPtr)();
  return -1U;
}
