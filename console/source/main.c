#include <stdint.h>
#include "framebuffer.h"
#include "stdio.h"
#include "timer.h"
#include "mmio.h"
#include "uart.h"
#include "shell.h"

#define WAIT_DELAY      0x7F0000

extern void* _heap_start;  // start address where we can put our data


void writeTextInBox(const char* text)  // only one Line (don't use newlines)
{
  uint32_t text_length = strlen(text) + 2;
  int32_t i;
  putchar(0xC9); for(i=0; i<text_length; ++i) putchar(0xCD); putchar(0xBB); putchar('\n');
  putchar(0xBA); putchar(' '); printf(text);  putchar(' '); putchar(0xBA); putchar('\n');         uartPutc('\n');
  putchar(0xC8); for(i=0; i<text_length; ++i) putchar(0xCD); putchar(0xBC); putchar('\n');
}

void main(void)
{
  //uint32_t fb_x = 640;
  //uint32_t fb_y = 480;
  //fbInit(fb_x, fb_y);
  //fbInitNativ();
  //uartInit();

  writeTextInBox("Welcome to Martins and Manuels Shell!!");

  printf("start address of heap: %x\n", &_heap_start);

  //setStdOutput(0);
  //printf("Monitor Resolution: X=%u / Y=%u\n",fb_x,fb_y);

  // More printable signs available ;)
  int32_t i;
  for(i=128;i<256;++i)
  {
    if(!(i & 0xF)) putchar('\n');
    putchar(i);
  }
  putchar('\n');

  //shell();

  while(1)
    uartPutc(uartGetc());

}

