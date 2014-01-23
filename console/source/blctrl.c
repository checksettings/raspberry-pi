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
/*
uint32_t SRF08getDistance(void)
{
  register_address = SRF08_VERSION_REGISTER;
  buffer[0] = SRF08_COMMAND_REGISTER;
  buffer[1] = SRF08_MEASURE_CM;
  i2cWrite(SRF08_DEFAULT_ADDRESS, 2, buffer);

  // wait some time
  delay(SRF08_WAITING);

  buffer[0] = 0xFF;
  buffer[1] = 0xFF;
  register_address = SRF08_DISTANCE_HIGH_REGISTER;

  i2cReadWithRegister(SRF08_DEFAULT_ADDRESS, &register_address, 2, buffer);

  return (buffer[0]<<8)+buffer[1];
}

*/
