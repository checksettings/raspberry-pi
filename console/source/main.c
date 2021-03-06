#include <stdint.h>
#include "framebuffer.h"
#include "stdio.h"
#include "stdlib.h"
#include "timer.h"
#include "mmio.h"
#include "uart.h"
#include "string.h"
#include "shell.h"

#include "i2c.h"
#include "sensorconfig.h"
#include "actorconfig.h"
#include "distanceAPI.h"
#include "directionAPI.h"
#include "motionAPI.h"

#include "motorAPI.h"

#define WAIT_DELAY      0x7F0000

void writeTextInBox(const char* text)  // only one Line (don't use newlines)
{
  uint32_t text_length = strlen(text) + 2;
  int32_t i;
  putchar(0xC9); for(i=0; i<text_length; ++i) putchar(0xCD); putchar(0xBB); putchar('\n');
  putchar(0xBA); putchar(' '); printf(text);  putchar(' '); putchar(0xBA); putchar('\n');
  putchar(0xC8); for(i=0; i<text_length; ++i) putchar(0xCD); putchar(0xBC); putchar('\n');
}

void initAPIs()
{
  i2cInit();
  distanceInit();
  directionInit();
  motionInit();
  motorInit();
#ifdef MAG3110
	getDirInit();
#endif
#ifdef MMA8452Q
	getMotInit();
#endif
}

void printSensorValues()
{
#ifdef SRF08
  printf("SRF08:  %3d\n",getDistance());
#endif
#ifdef CMPS10
  printf("CMPS10: %3d\n",getBearing()/10);
#endif
#ifdef MAG3110
  uint16_t direction[3];
  getDirection(direction);
  printf("Measured Direction: x [ %d ] y [ %d ] z [ %d ]\n",direction[0],direction[1],direction[2]);
#endif
#ifdef MMA8452Q
  uint16_t motion[3];
  getMotion(motion);
  printf("Measured Motion: x [ %d ] y [ %d ] z [ %d ]\n",motion[0],motion[1],motion[2]);
#endif
}

void printSensorVersions()
{
#ifdef SRF08
  printf("SRF08:  %d\n",getDistVersion());
#endif
#ifdef CMPS10
  printf("CMPS10: %d\n",getDirVersion());
#endif
#ifdef MAG3110
  printf("MAG3110: %d\n",getDirVersion());
#endif
#ifdef MMA8452Q
  printf("MMA8452Q: %d\n",getMotVersion());
#endif
}

void magnetometer()
{
#ifdef MAG3110
	int count = 100;
  i2cInit();
	directionInit();
	getDirInit();
  printf("MAG3110: %d\n",getDirVersion());
	while(count > 1)
	{
		count--;
		uint16_t direction[3];
		direction[0] = 0;
		direction[1] = 0;
		direction[2] = 0;
		getDirection(direction);
		printf("Measured Direction: x [ %d ] y [ %d ] z [ %d ], Temp: %d \n",direction[0],direction[1],direction[2],getTemperature());
	}
#else
  printf("MAG3110 is not included!\n");
#endif
}

void startMotor()
{
  int i = 0;
  for(i = 0;i < 30;i++)
  {
    setMotorSpeed(30);
    delay(0x70000);
  }
  for(i = 0;i < 30;i++)
  {
    setMotorSpeed(100);
    delay(0x70000);
  }
  for(i = 0;i < 30;i++)
  {
    setMotorSpeed(255);
    delay(0x70000);
  }

}

void accelerometer()
{
#ifdef MMA8452Q
	int count = 100;
  i2cInit();
	motionInit();
	getMotInit();
  printf("MMA8452Q: %d\n",getMotVersion());
	while(count > 1)
	{
		count--;
		uint16_t motion[3];
		motion[0] = 0;
		motion[1] = 0;
		motion[2] = 0;
		getMotion(motion);
		printf("Measured Motion: x [ %d ] y [ %d ] z [ %d ]\n",motion[0],motion[1],motion[2]);
	}
#else
  printf("MMA8452Q is not included!\n");
#endif
}

void main(void)
{
  uartInit();
  char* myhelp="this is mein main function";
  addNewCommand(newcommandstoadd,"startmy",myhelp);

  char* help_api_init = "init i2c and APIs";
  addNewCommand(initAPIs,"i",help_api_init);

  char* help_sensor_values = "get values";
  addNewCommand(printSensorValues,"m",help_sensor_values);

  char* help_sensor_versions = "get versions";
  addNewCommand(printSensorVersions,"v",help_sensor_versions);

  char* help_sensor_mag = "get magnetometer values";
  addNewCommand(magnetometer,"mag",help_sensor_mag);

  char* help_sensor_acc = "get accelerometer values";
  addNewCommand(accelerometer,"acc",help_sensor_acc);

  char* help_actor_motor = "start motor";
  addNewCommand(startMotor,"blctrl",help_actor_motor);

  //uint32_t fb_x = 640;
  //uint32_t fb_y = 480;
  //fbInit(fb_x, fb_y, COLORMODE_32BIT);
//  setStdOutput(OUTPUT_MONITOR);
  //fbInitNativ();

//  writeTextInBox("Welcome to Martins and Manuels Shell!!");
  // change foreground/background color (example)
  //rgb foreground = {.rgb_16.rgb = COLOR16_RED};
  //rgb background = {.rgb_16.rgb = COLOR16_YELLOW};
  //rgb foreground = {.rgb_ = COLOR32_RED};
  //rgb background = {.rgb_ = COLOR32_YELLOW};
  //consoleForegroundColor(foreground);
  //consoleBackgroundColor(background);

//  writeTextInBox("Welcome to Martins and Manuels Shell!!");
  printf("Welcome to Martins and Manuels Shell!! \n");
/*
  printf("start address of heap: %x\n", &_heap_start);

  uint32_t* test1 = malloc(5*sizeof(uint32_t));
  printf("test1: %x\n", test1);
  uint32_t* test2 = malloc(10*sizeof(uint32_t));
  //test2[10] = 55;  // to test an error
  printf("test2: %x\n", test2);
  uint32_t* test3 = malloc(20*sizeof(uint32_t));
  printf("test3: %x\n", test3);

  free(test2);
  test2 = malloc(3*sizeof(uint32_t));
  printf("test2: %x\n", test2);
  uint32_t* test4 = malloc(2*sizeof(uint32_t));
  printf("test4: %x\n", test4);
  free(test4);
  test2 = realloc(test2, 10*sizeof(uint32_t));
  printf("test2: %x\n", test2);
  test2 = realloc(test2, 11*sizeof(uint32_t));
  printf("test2: %x\n", test2);
  test4 = malloc(5*sizeof(uint32_t));
  printf("test4: %x\n", test4);


  //setStdOutput(OUTPUT_UART);
  //printf("Monitor Resolution: X=%u / Y=%u\n",fb_x,fb_y);

  // More printable signs available on monitor output ;)
  int32_t i;
  for(i=128;i<256;++i)
  {
    if(!(i & 0xF)) putchar('\n');
    putchar(i);
  }
  putchar('\n');
*/

  shell();
//  printf("wieder in der main am ende\n");

//  while(1)
  //  uartPutc(uartGetc());

}
