#include "motorAPI.h"
#include "actorconfig.h"

#include "stdio.h"

#include "blctrl.h"

uint32_t (*getMotorVersionPtr)();
uint32_t (*setMotorSpeedPtr)();
uint32_t (*getMotorSpeedPtr)();


void motorInit(void)
{
  printf("motorAPIInit\n");
  getMotorVersionPtr = 0;
  setMotorSpeedPtr = 0;
  getMotorSpeedPtr = 0;
  #ifdef BLCTRL
    printf("- BLCTRL\n");
    setMotorSpeedPtr = &BLCTRLsetMotorSpeed;
  #endif
}

uint32_t getMotorSpeed(void)
{
  if (getMotorSpeedPtr != 0)
    return (*getMotorSpeedPtr)();
  return -1U;
}

uint32_t setMotorSpeed(uint32_t value)
{
  if (setMotorSpeedPtr != 0)
    return (*setMotorSpeedPtr)(value);
  return -1U;
}

uint32_t getMotorVersion(void)
{
  if (getMotorVersionPtr != 0)
    return (*getMotorVersionPtr)();
  return -1U;
}
