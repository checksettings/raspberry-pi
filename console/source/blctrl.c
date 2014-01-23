#include "stdio.h"
#include "i2c.h"
#include "blctrl.h"

#define MAX_LEN 2
#define MAX_SPEED 255


char buffer[MAX_LEN];
char register_address;


uint32_t BLCTRLsetMotorSpeed(uint32_t value)
{
  buffer[0] = (MAX_SPEED < value)?(MAX_SPEED):(value);
  
  i2cWrite(BLCTRL_DEFAULT_ADDRESS, 1, buffer);

  return value;
}

