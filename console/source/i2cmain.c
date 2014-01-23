#include "i2cmain.h"
#include "stdio.h"
#include "timer.h"
#include "i2c.h"
#include "mma8452q.h"
#include "mag3110.h"

#define MAX_LEN 32

uint8_t bufIn[MAX_LEN];
char buffer[MAX_LEN];
char register_address;

void i2cmain()
{
  uint32_t ret_val = 0;
  buffer[0] = 0xFF;
	printf("i2cmain start\n");
  i2cInit();

	printf("Read MMA8452Q\n");
 // talk to MMA8452Q
 register_address = MMA8452Q_OUT_X_MSB;
  buffer[0] = 0xFF;
  ret_val = i2cReadWithRegister(MMA8452Q_DEFAULT_ADDRESS, &register_address, 1, buffer);  
	printf("MMA8452Q version: %x sollte 0x2A sein\n",buffer[0]);
	if (ret_val != 0)
		printf("PROBLEM\n"); 

// wait some time
  delay(0xF0000);

	
	printf("i2cmain end\n");
  i2cClose();
}
