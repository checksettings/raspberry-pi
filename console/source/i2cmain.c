#include "i2cmain.h"
#include "stdio.h"
#include "i2c.h"

#define MAX_LEN 32

#define addressMMA8452Q 0x1C
#define address_XLOBORG_Compass 0x0E
#define addressSRF08 0xE0
#define addressCMPS10 0xC0


char buffer[MAX_LEN];
char register_address;

void i2cmain()
{
  uint32_t ret_val;
  buffer[0] = 0xFF;
	printf("i2cmain start\n");
	
  printf("i2cmain init\n");
  i2cInit();

/*
  // talk to srf08
  printf("i2cmain read\n");
  ret_val = i2cReadWithRegister(addressSRF08, 0,1, buffer);  
	printf("return: %d\n",ret_val);
	printf("SRF08 version %d\n",buffer[0]);

  // talk to CMPS10
  register_address = 0x01;
  ret_val = i2cReadWithRegister(addressCMPS10, &register_address , 1, buffer);
	printf("return: %d\n",ret_val);
	printf("CMPS10 version %x : %x\n",buffer[0],buffer[1]);
*/
  // talk to MMA8452Q
  printf("i2cmain read\n");
  register_address = 0x0D;
  buffer[0] = 0xFF;
  ret_val = i2cReadWithRegister(addressMMA8452Q, &register_address, 1, buffer);  
	printf("return: %d\n",ret_val);
	printf("MMA8452Q version: %x sollte 0x2A sein\n",buffer[0]);

	printf("i2cmain end\n");
  i2cClose();
}
