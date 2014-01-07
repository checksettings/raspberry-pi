#include "positionAPI.h"
#include "sensorconfig.h"

#include "stdio.h"

#include "bmp085.h"

uint32_t (*posGetVersionPtr)();
uint32_t (*posGetAltitudePtr)();

void positionInit(void)
{
  printf("distanceAPI\n");
  posGetVersionPtr = 0;
  posGetAltitudePtr = 0;
  #ifdef BMP085
    printf("- BMP085\n");
    posGetVersionPtr = &BMP085getVersion;
    posGetAltitudePtr = &BMP085getAltitude;
    BMP085init(); //AC1: 8671 version: 02 id: 0x55(85)
  #endif
}

uint32_t posGetAltitude(void)
{
  if (posGetAltitudePtr != 0)
    return (*posGetAltitudePtr)();
  return -1U;
}

uint32_t posGetVersion(void)
{
  if (posGetVersionPtr != 0)
    return (*posGetVersionPtr)();
  return -1U;
}
