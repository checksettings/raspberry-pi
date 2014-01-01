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
#include "distanceAPI.h"
#include "magnetometerAPI.h"

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
  magnetometerInit();
}

void printSensorValues()
{
  printf("SRF08:  %3d\n",disGetDistance());
  printf("CMPS10: %3d\n",magGetBearing()/10);
}

void printSensorVersions()
{
  printf("SRF08:  %d\n",disGetVersion());
  printf("CMPS10: %d\n",magGetVersion());
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
