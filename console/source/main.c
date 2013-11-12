#include <stdint.h>
#include "framebuffer.h"
#include "stdio.h"
#include "timer.h"
#include "mmio.h"
#include "uart.h"
#include "shell.h"

#define WAIT_DELAY      0x7F0000


void main(void)
{
  uint32_t fb_x = 640;
  uint32_t fb_y = 480;
  fbInit(fb_x, fb_y);
  //fbInitNativ();
  uartInit();

  puts("This is my first Output on console!!\n");

  //setStdOutput(0);
  printf("Monitor Resolution: X=%u / Y=%u\n",fb_x,fb_y);

  //shell();

  while(1)
    putchar(uartGetc());

}

