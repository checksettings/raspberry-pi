#include "stdio.h"
#include "timer.h"
#include "i2c.h"
#include "cmps10.h"

#define MAX_LEN 32


char buffer[MAX_LEN];
char register_address;

void cmps10Main()
{
  uint32_t ret_val;
  buffer[0] = 0xFF;
	printf("i2cmain start\n");
	
  printf("i2cmain init\n");
  i2cInit();

  // Start talking to cmps10
  register_address = CMPS10_VERSION_REGISTER;
  printf("Get CMPS10 Version\n");
  ret_val = i2cReadWithRegister(CMPS10_DEFAULT_ADDRESS, &register_address, 1, buffer);  
	printf("return: %d\n",ret_val);
	printf("CMPS10 version is %x \n",buffer[0]);
  printf("starting to measuring\n");

  while(1)
  {
    // read measurement
    register_address = CMPS10_BEARING_REGISTER;
    ret_val = i2cReadWithRegister(CMPS10_DEFAULT_ADDRESS, &register_address, 5, buffer);  
	  printf("Bearing: %d, %d, Pitch: %d, Roll: %d\n",buffer[0], (buffer[1]<<8)+buffer[2], buffer[3], buffer[4]);

    // wait some time
    delay(CMPS10_WAITING);
  }
  
  // won't be reached here 
	printf("i2cmain end\n");
  i2cClose();
}
