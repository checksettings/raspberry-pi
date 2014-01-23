#include "directionAPI.h"
#include "sensorconfig.h"

#include "stdio.h"

#include "mag3110.h"
#include "cmps10.h"

uint32_t (*getDirInitPtr)();
uint32_t (*getDirVersionPtr)();
uint32_t (*getDirectionPtr)();
uint32_t (*getTemperaturePtr)();
uint32_t (*getBearingPtr)();

void directionInit(void)
{
  printf("directionInit\n");
  getDirVersionPtr = 0;
  getDirectionPtr = 0;
	getTemperaturePtr = 0;
	getDirInitPtr = 0;
  getBearingPtr = 0;
  #ifdef CMPS10
    printf("- CMPS10\n");
    getDirVersionPtr = &CMPS10getVersion;
    getBearingPtr = &CMPS10getBearing;

  #else
  #ifdef MAG3110
    printf("- MAG3110\n");
		getDirInitPtr = &MAG3110getInit;
    getDirVersionPtr = &MAG3110getVersion;
    getDirectionPtr = &MAG3110getDirection;
		getTemperaturePtr = &MAG3110getTemperature;
  #endif
  #endif
}

uint32_t getDirInit(void)
{
  if (getDirInitPtr != 0)
    return (*getDirInitPtr)();
  return -1U;
}

uint32_t getTemperature(void)
{
  if (getTemperaturePtr != 0)
    return (*getTemperaturePtr)();
  return -1U;
}

uint32_t getDirection(uint16_t* directions)
{
  if (getDirectionPtr != 0)
    return (*getDirectionPtr)();
  return -1U;
}

uint32_t getDirVersion(void)
{
  if (getDirVersionPtr != 0)
    return (*getDirVersionPtr)();
  return -1U;
}

uint32_t getBearing(void)
{
  if (getBearingPtr != 0)
    return (*getBearingPtr)();
  return -1U;
}
