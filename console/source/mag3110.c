#include "stdio.h"
#include "timer.h"
#include "i2c.h"
#include "mag3110.h"

#define MAX_LEN 32

char buffer[MAX_LEN];
char register_address;

uint32_t MAG3110getInit(void)
{
	uint16_t ret_val;

	// INIT COMPASS MAG3110
  // Set to standby mode for changing
  buffer[0] = MAG3110_CTRL_REG_1;
  buffer[1] = 0x00; // Standby Mode

	ret_val = i2cWrite(MAG3110_DEFAULT_ADDRESS, 2, buffer);
  if (ret_val != 0) {
  	printf("Failed sending CTRL_REG1!\n");
		printf("return: %x\n",ret_val);
		return ret_val;
  }

  // Acquisition mode
  buffer[0] = MAG3110_CTRL_REG_2;
  buffer[1] = (1 << 7) | (1 << 5) | (0 << 4); //0x80; is simple mod
              // 1 0 1 0 | 0 0 0 0  == 0x90
              // Reset before each acquisition
              // Raw mode, do not apply user offsets
              // Disable reset cycle
	ret_val = i2cWrite(MAG3110_DEFAULT_ADDRESS, 2, buffer);
  if (ret_val != 0) {
  	printf("Failed sending CTRL_REG2!\n");
		printf("return: %x\n",ret_val);
		return ret_val;
  }
  
	// System operation
  buffer[0] = MAG3110_CTRL_REG_1;
  buffer[1] = (0 << 5) | (3 << 3) | (0 << 2) | (0 << 1) | (1 << 0); //0x01; is simple mod
              // 0 0 1 1 | 1 0 0 1 == 0x38
              // Output data rate (10 Hz when paired with 128 oversample)
              // Oversample of 128
              // Disable fast read
              // Continuous measurement
              // Active mode

	ret_val = i2cWrite(MAG3110_DEFAULT_ADDRESS, 2, buffer);
  if (ret_val != 0) {
  	printf("Failed sending CTRL_REG1!\n");
		printf("return: %x\n",ret_val);
		return ret_val;
  }
	
  printf("MAG3310 initialised!\n");
	return 0;
}

uint32_t MAG3110getVersion(void)
{
  buffer[0] = 0xFF;
  buffer[7] = 0x02;
  register_address = MAG3110_WHO_AM_I;
  i2cReadWithRegister(MAG3110_DEFAULT_ADDRESS, &register_address, 1, buffer);
  //i2cRead(MAG3110_DEFAULT_ADDRESS, 10, buffer);
  return buffer[0];
}

uint32_t MAG3110getDirection(uint16_t* directions)
{
  int16_t x = 0;
  int16_t y = 0;
  int16_t z = 0;

	buffer[0] = 0x00;

	register_address = MAG3110_OUT_X_MSB;
	//i2cReadWithRegister(, &register_address, 7, buffer);
	//i2cWrite(MAG3110_DEFAULT_ADDRESS, 1, buffer);
  //i2cRead(MAG3110_DEFAULT_ADDRESS, 18, buffer);
	i2cReadWithRegister(MAG3110_DEFAULT_ADDRESS,&register_address, 7, buffer);

  *((uint16_t*)&x) = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
  *((uint16_t*)&y) = ((uint16_t)buffer[2] << 8) | (uint16_t)buffer[3];
  *((uint16_t*)&z) = ((uint16_t)buffer[4] << 8) | (uint16_t)buffer[5];

  if (directions != 0) {
      directions[0] = x;
      directions[1] = y;
      directions[2] = z;
  }

	return 0;
}

uint32_t MAG3110getTemperature(void)
{
	int8_t temp;

	buffer[0] = 0xFF;
  register_address = MAG3110_TEMPERATURE;
	i2cReadWithRegister(MAG3110_DEFAULT_ADDRESS, &register_address, 1, buffer);
	*((uint8_t*)&temp) = buffer[0];
	return temp;
}

