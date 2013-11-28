#include "i2cmain.h"
#include "stdio.h"
#include "i2c.h"

#define MAX_LEN 32

char buffer[MAX_LEN];


void i2cmain()
{
  uint32_t ret_val;
  buffer[0] = 0xFF;
	printf("i2cmain start\n");
	
  printf("i2cmain init\n");
  i2cInit();

  printf("i2cmain read\n");
  ret_val = i2cReadWithRegister(0xE0, 0,1, buffer);
  
	printf("return: %d\n",ret_val);
	printf("SRF08 version %d\n",buffer[0]);

  ret_val = i2cReadWithRegister(0xC0, 0,2, buffer);
	printf("return: %d\n",ret_val);
	printf("CMPS10 version %d : %d\n",buffer[0],buffer[1]);

	printf("i2cmain end\n");
  i2cClose();
}
