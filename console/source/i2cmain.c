#include "i2cmain.h"
#include "stdio.h"
#include "timer.h"
#include "i2c.h"

#define MAX_LEN 32

#define ADDRESS_MMA8452Q 0x1C
#define ADDRESS_XLOBORG_Compass 0x0E
#define ADDRESS_SRF08 0xE0
#define ADDRESS_CMPS10 0xC0

/// ------------------------- SRF08
#define SRF08_MEASURE_INCH            0x50 // Measure start command (inch)
#define SRF08_MEASURE_CM              0x51 // Measure start command (centimeter)
#define SRF08_MEASURE_US              0x52 // Measure start command (microsecondes)
#define SRF08_ANN_MEASURE_INCH        0x53 // ANN-Mode Measure start command (inch)
#define SRF08_ANN_MEASURE_CM          0x54 // ANN-Mode Measure start command (centimeter)
#define SRF08_ANN_MEASURE_US          0x55 // ANN-Mode Measure start command (microsecondes)

// Register for sending commands
#define SRF08_COMMAND_REGISTER        0x00 // Register for writing the measure command to start measure process

// Register for reading commands
#define SRF08_VERSION_REGISTER        0x00 // Register for reading the software version of sensor
#define SRF08_BRIGHTNESS_REGISTER     0x01 // Register for reading the brightness value
#define SRF08_DISTANCE_HIGH_REGISTER  0x02 // Register for reading the high-byte of last measured distance
#define SRF08_DISTANCE_LOW_REGISTER   0x03 // Register for reading the low-byte of last measured distance

// Register for ANN-Mode reading
#define SRF08_ANN01_HIGH_REGISTER     0x02 // Register for reading the high-byte of 1. ANN measure
#define SRF08_ANN01_LOW_REGISTER      0x03 // Register for reading the low-byte of 1. ANN measure
#define SRF08_ANN02_HIGH_REGISTER     0x04 // Register for reading the high-byte of 2. ANN measure
#define SRF08_ANN02_LOW_REGISTER      0x05 // Register for reading the low-byte of 2. ANN measure
#define SRF08_ANN03_HIGH_REGISTER     0x06 // Register for reading the high-byte of 3. ANN measure
#define SRF08_ANN03_LOW_REGISTER      0x07 // Register for reading the low-byte of 3. ANN measure
#define SRF08_ANN04_HIGH_REGISTER     0x08 // Register for reading the high-byte of 4. ANN measure
#define SRF08_ANN04_LOW_REGISTER      0x09 // Register for reading the low-byte of 4. ANN measure
#define SRF08_ANN05_HIGH_REGISTER     0x0A // Register for reading the high-byte of 5. ANN measure
#define SRF08_ANN05_LOW_REGISTER      0x0B // Register for reading the low-byte of 5. ANN measure
#define SRF08_ANN06_HIGH_REGISTER     0x0C // Register for reading the high-byte of 6. ANN measure
#define SRF08_ANN06_LOW_REGISTER      0x0D // Register for reading the low-byte of 6. ANN measure
#define SRF08_ANN07_HIGH_REGISTER     0x0E // Register for reading the high-byte of 7. ANN measure
#define SRF08_ANN07_LOW_REGISTER      0x0F // Register for reading the low-byte of 7. ANN measure
#define SRF08_ANN08_HIGH_REGISTER     0x10 // Register for reading the high-byte of 8. ANN measure
#define SRF08_ANN08_LOW_REGISTER      0x11 // Register for reading the low-byte of 8. ANN measure
#define SRF08_ANN09_HIGH_REGISTER     0x12 // Register for reading the high-byte of 9. ANN measure
#define SRF08_ANN09_LOW_REGISTER      0x13 // Register for reading the low-byte of 9. ANN measure
#define SRF08_ANN10_HIGH_REGISTER     0x14 // Register for reading the high-byte of 10. ANN measure
#define SRF08_ANN10_LOW_REGISTER      0x15 // Register for reading the low-byte of 10. ANN measure
#define SRF08_ANN11_HIGH_REGISTER     0x16 // Register for reading the high-byte of 11. ANN measure
#define SRF08_ANN11_LOW_REGISTER      0x17 // Register for reading the low-byte of 11. ANN measure
#define SRF08_ANN12_HIGH_REGISTER     0x18 // Register for reading the high-byte of 12. ANN measure
#define SRF08_ANN12_LOW_REGISTER      0x19 // Register for reading the low-byte of 12. ANN measure
#define SRF08_ANN13_HIGH_REGISTER     0x1A // Register for reading the high-byte of 13. ANN measure
#define SRF08_ANN13_LOW_REGISTER      0x1B // Register for reading the low-byte of 13. ANN measure
#define SRF08_ANN14_HIGH_REGISTER     0x1C // Register for reading the high-byte of 14. ANN measure
#define SRF08_ANN14_LOW_REGISTER      0x1D // Register for reading the low-byte of 14. ANN measure
#define SRF08_ANN15_HIGH_REGISTER     0x1E // Register for reading the high-byte of 15. ANN measure
#define SRF08_ANN15_LOW_REGISTER      0x1F // Register for reading the low-byte of 15. ANN measure
#define SRF08_ANN16_HIGH_REGISTER     0x20 // Register for reading the high-byte of 16. ANN measure
#define SRF08_ANN16_LOW_REGISTER      0x21 // Register for reading the low-byte of 16. ANN measure
#define SRF08_ANN17_HIGH_REGISTER     0x22 // Register for reading the high-byte of 17. ANN measure
#define SRF08_ANN17_LOW_REGISTER      0x23 // Register for reading the low-byte of 17. ANN measure

#define SRF08_ANN_NUM 17

char buffer[MAX_LEN];
char register_address;

void i2cmain()
{
  // I2C prototypes
  // read
  // uint8_t i2cRead(uint8_t address, uint32_t bytes, char *data)
  // uint8_t i2cReadWithRegister(uint8_t address, const char *reg_address, uint32_t bytes, char *data)
  // write
  // uint8_t i2cWrite(uint8_t address, uint32_t bytes, const char *data)
  
  uint32_t ret_val;
  buffer[0] = 0xFF;
	printf("i2cmain start\n");
	
  printf("i2cmain init\n");
  i2cInit();

  // Start talking to srf08
  register_address = 0x00;
  printf("Get SRF08 Version\n");
  ret_val = i2cReadWithRegister(ADDRESS_SRF08, &register_address, 1, buffer);  
	printf("return: %d\n",ret_val);
	printf("SRF08 version is %x \n",buffer[0]);


  while(1)
{
  // set srf08 to measurement
//  printf("Set SRF08 to measure the distance\n");
  buffer[0] = SRF08_COMMAND_REGISTER;
  buffer[1] = SRF08_MEASURE_CM;
  i2cWrite(ADDRESS_SRF08, 2, buffer);
  
  // wait some time
  delay(0xF0000);

  // read measurement
  buffer[0] = 0xFF;
  buffer[1] = 0xFF;
  register_address = SRF08_DISTANCE_HIGH_REGISTER;
  //printf("Read the distance from SRF08\n");
  ret_val = i2cReadWithRegister(ADDRESS_SRF08, &register_address, 2, buffer);  
	//printf("return: %d\n",ret_val);
	printf("Measured distance: %d \n",(buffer[0]<<8)+buffer[1]);

  // wait some time
  delay(0xF0000);
}
  //
/*
  // talk to CMPS10
  register_address = 0x00;
  ret_val = i2cReadWithRegister(ADDRESS_CMPS10, &register_address , 1, buffer);
	printf("return: %d\n",ret_val);
	printf("CMPS10 version %x \n",buffer[0]);

 
  // talk to MMA8452Q
  printf("i2cmain read\n");
  register_address = 0x0D;
  buffer[0] = 0xFF;
  ret_val = i2cReadWithRegister(ADDRESS_MMA8452Q, &register_address, 1, buffer);  
	printf("return: %d\n",ret_val);
	printf("MMA8452Q version: %x sollte 0x2A sein\n",buffer[0]);
*/
	printf("i2cmain end\n");
  i2cClose();
}
