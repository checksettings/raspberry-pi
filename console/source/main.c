#include <stdint.h>
#include "framebuffer.h"
#include "uart.h"
#include "mmio.h"


#define WAIT_DELAY      0x3F0000


void delayWithLoop(uint32_t value);

void main()
{
  //uint32_t fb_x = 640;
  //uint32_t fb_y = 480;
  //fbInit(fb_x, fb_y);
  //fbInitNativ();

  //consoleWrite("This is my first Output on console!!");

  // set as output
  //mmio32(GPFSEL1_OFFS) |= (1<<LED_BIT_SET);

  //while(1)
  //{
    //// switch on led
    //mmio32(GPCLR0_OFFS) |= (1<<GPIO_PIN_OFFS);

    //// wait 1
    //delayWithLoop(WAIT_DELAY);

    //// switch off led
    //mmio32(GPSET0_OFFS) |= (1<<GPIO_PIN_OFFS);

    //// wait 2
    //delayWithLoop(WAIT_DELAY);
  //}

  uartInit();

  char input[7];
  char test[] ={'g','i','b',' ','w','a','s',' ','e','i','n','\n','\0'};
  uartPuts(test);
  int count = 6;
  while(count>0)
  {
    input[6-count] =uartGetc();
    count-- ;
  }
  input[6] = 0;
  uartPuts(input);

}

void delayWithLoop(volatile uint32_t value)
{
  for(;  value != 0; value--);
}
