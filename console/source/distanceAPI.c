#include "distanceAPI.h"
#include "sensorconfig.h"

#include "stdio.h"

#include "srf08.h"

uint32_t (*getVersionPtr)();
uint32_t (*getDistancePtr)();

void distanceInit(void)
{
  printf("distanceInit\n");
  getVersionPtr = 0;
  getDistancePtr = 0;
  #ifdef SRF08
    printf("- SRF08\n");
    getVersionPtr = &SRF08getVersion;
    getDistancePtr = &SRF08getDistance;
  #endif
}

uint32_t getDistance(void)
{
  if (getDistancePtr != 0)
    return (*getDistancePtr)();
  return -1U;
}

uint32_t getVersion(void)
{
  if (getVersionPtr != 0)
    return (*getVersionPtr)();
  return -1U;
}
