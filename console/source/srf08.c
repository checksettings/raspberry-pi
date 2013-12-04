#include "stdio.h"
#include "timer.h"
#include "i2c.h"
#include "srf08.h"

#define MAX_LEN 32


char buffer[MAX_LEN];
char register_address;

void srf08Main()
{
  uint32_t ret_val;
  buffer[0] = 0xFF;
	printf("i2cmain start\n");
	
  printf("i2cmain init\n");
  i2cInit();

  // Start talking to srf08
  register_address = SRF08_VERSION_REGISTER;
  printf("Get SRF08 Version\n");
  ret_val = i2cReadWithRegister(SRF08_DEFAULT_ADDRESS, &register_address, 1, buffer);  
	printf("return: %d\n",ret_val);
	printf("SRF08 version is %x \n",buffer[0]);
  printf("starting to measure the distance\n");

  while(1)
  {
    // set srf08 to measurement
    // printf("Set SRF08 to measure the distance\n");
    buffer[0] = SRF08_COMMAND_REGISTER;
    buffer[1] = SRF08_MEASURE_CM;
    i2cWrite(SRF08_DEFAULT_ADDRESS, 2, buffer);
    
    // wait some time
    delay(SRF08_WAITING);

    // read measurement
    buffer[0] = 0xFF;
    buffer[1] = 0xFF;
    register_address = SRF08_DISTANCE_HIGH_REGISTER;
    //printf("Read the distance from SRF08\n");
    ret_val = i2cReadWithRegister(SRF08_DEFAULT_ADDRESS, &register_address, 2, buffer);  
	  //printf("return: %d\n",ret_val);
	  printf("Measured distance: %d \n",(buffer[0]<<8)+buffer[1]);

    // wait some time
    delay(SRF08_WAITING);
  }
  
  // won't be reached here 
	printf("i2cmain end\n");
  i2cClose();
}
