#include "stdio.h"
#include "timer.h"
#include "i2c.h"
#include "mma8452q.h"

#define MAX_LEN 32

char buffer[MAX_LEN];
char register_address;
uint32_t gCount = 0;

uint32_t MMA8452QgetInit(void)
{
	uint16_t ret_val;

	// INIT Accelerometer MMA8452Q
  // Setup mode configuration
  buffer[0] = MMA8452Q_CTRL_REG_1; 
  buffer[1] = (1 << 7) | (1 << 6) | (0 << 4) | (0 << 2) | (1 << 1) | (1 << 0);
              // Sleep rate 50 Hz
              // Data rate 800 Hz
              // No reduced noise mode
              // Normal read mode
              // Active
  ret_val = i2cWrite(MMA8452Q_DEFAULT_ADDRESS, 2, buffer);
  if (ret_val != 0) {
    printf("Failed sending CTRL_REG1!\n");
		printf("return: %x\n",ret_val);
		return ret_val;
  }

  // Setup range
  buffer[0] = MMA8452Q_XYZ_DATA_CFG;
  buffer[1] = MMA8452Q_RANGE_2G_NHPF;
	ret_val = i2cWrite(MMA8452Q_DEFAULT_ADDRESS, 2, buffer);
  if (ret_val != 0) {
    printf("Failed sending XYZ_DATA_CFG!\n");
		printf("return: %x\n",ret_val);
		return ret_val;
  }
  
  // 2G over 128 counts
	gCount = 2000 / 128; 

  // System state
  buffer[0] = MMA8452Q_SYSMOD;
  buffer[1] = MMA8452Q_WAKE_MOD;
	ret_val = i2cWrite(MMA8452Q_DEFAULT_ADDRESS, 2, buffer);
  if (ret_val != 0) {
    printf("Failed sending SYSMOD!\n");
		printf("return: %x\n",ret_val);
		return ret_val;
  }

  // Reset ready for reading
  buffer[0] = 0xFF;
	ret_val = i2cWrite(MMA8452Q_DEFAULT_ADDRESS, 1, buffer);
  if (ret_val != 0) {
    printf("Failed sending final write!\n");
		printf("return: %x\n",ret_val);
		return ret_val;
  }

  printf("MMA8452Q initialised!\n");
	return 0;
}

uint32_t MMA8452QgetVersion(void)
{
  buffer[0] = 0xFF;
  register_address = MMA8452Q_WHO_AM_I;
  i2cReadWithRegister(MMA8452Q_DEFAULT_ADDRESS, &register_address, 1, buffer);
  return buffer[11];
}

uint32_t MMA8452QgetMotion(uint16_t* motions)
{
  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t z = 0;

	buffer[0] = 0x00;
	register_address = MMA8452Q_OUT_X_MSB;
	i2cReadWithRegisterRS(MMA8452Q_DEFAULT_ADDRESS, &register_address, 4, buffer);

  *((uint8_t*)&x) = buffer[1];
  *((uint8_t*)&y) = buffer[2];
  *((uint8_t*)&z) = buffer[3];

  // Convert to Gs wrong normalized, should be true = x / 1000
	if (motions != 0) {
      motions[0] = x;// * gCount;
      motions[1] = y;// * gCount;
      motions[2] = z;// * gCount;
  }

	return 0;
}
