#include "motionAPI.h"
#include "sensorconfig.h"

#include "stdio.h"
#include "mma8452q.h"

uint32_t (*getMotInitPtr)();
uint32_t (*getMotVersionPtr)();
uint32_t (*getMotionPtr)();

void motionInit(void)
{
  printf("motionInit\n");
  getMotVersionPtr = 0;
  getMotionPtr = 0;
  #ifdef MMA8452Q
    printf("- MMA8452Q\n");
		getMotInitPtr = &MMA8452QgetInit;
    getMotVersionPtr = &MMA8452QgetVersion;
    getMotionPtr = &MMA8452QgetMotion;
  #endif
}

uint32_t getMotInit(void)
{
  if (getMotInitPtr != 0)
    return (*getMotInitPtr)();
  return -1U;
}

uint32_t getMotion(uint16_t* motions)
{
  if (getMotionPtr != 0)
    return (*getMotionPtr)();
  return -1U;
}

uint32_t getMotVersion(void)
{
  if (getMotVersionPtr != 0)
    return (*getMotVersionPtr)();
  return -1U;
}
