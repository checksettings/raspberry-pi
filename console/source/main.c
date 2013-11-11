#include <stdint.h>
#include "framebuffer.h"
#include "timer.h"
#include "mmio.h"
//#include "uart.h"


#define WAIT_DELAY      0x3F0000


void main()
{
  uint32_t fb_x = 640;
  uint32_t fb_y = 480;
  fbInit(fb_x, fb_y);
  //fbInitNativ();

  consoleWrite("This is my first Output on console!!\n");
  uint8_t text[] = {'g','i','b',' ','w','a','s',' ','e','i','n','!'};
  int32_t i;
  for(i=0;i<12;++i)
  {
    consoleWriteChar(text[i]);
  }

  // set as output
  mmio32(GPFSEL1_OFFS) |= (1<<LED_BIT_SET);

  while(1)
  {
    // switch on led
    mmio32(GPCLR0_OFFS) |= (1<<GPIO_PIN_OFFS);

    // wait 1
    delay(WAIT_DELAY);

    // switch off led
    mmio32(GPSET0_OFFS) |= (1<<GPIO_PIN_OFFS);

    // wait 2
    delay(WAIT_DELAY);
  }

  //uartInit();
//
  //char input[7];
  //char test[] ={'g','i','b',' ','w','a','s',' ','e','i','n','\n','\0'};
  //uartPuts(test);
  //int count = 6;
  //while(count>0)
  //{
    //input[6-count] =uartGetc();
    //count-- ;
  //}
  //input[6] = 0;
  //uartPuts(input);

}

