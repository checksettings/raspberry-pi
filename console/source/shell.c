#include "shell.h"
#include "uart.h"
#include "stdio.h"


void shell(void)
{
  printf("Starting comandline\n");

  uint32_t exit = 1;
  uint32_t len = 0;
  char input[64];

  while(exit)
  {
    do
    {
      input[len] = uartGetc();
      printf("zeichen: %x",input[len]);
      //if(input[len] = 0x7f)

      //else
      uartPutc(input[len]);
    } while(input[len++] != '\n');
    //uartPutc('\n');
    input[len] = 0;
    len=0;
    printf(input);

    //exit =0;
  }
  printf("Close comandline");
}

