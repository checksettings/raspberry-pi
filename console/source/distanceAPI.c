#include "distanceAPI.h"
#include "sensorconfig.h"

#include "stdio.h"

#include "srf08.h"

uint32_t (*disGetVersionPtr)();
uint32_t (*disGetDistancePtr)();

void distanceInit(void)
{
  printf("distanceAPI\n");
  disGetVersionPtr = 0;
  disGetDistancePtr = 0;
  #ifdef SRF08
    printf("- SRF08\n");
    disGetVersionPtr = &SRF08getVersion;
    disGetDistancePtr = &SRF08getDistance;
  #endif
}

uint32_t disGetDistance(void)
{
  if (disGetDistancePtr != 0)
    return (*disGetDistancePtr)();
  return -1U;
}

uint32_t disGetVersion(void)
{
  if (disGetVersionPtr != 0)
    return (*disGetVersionPtr)();
  return -1U;
}
