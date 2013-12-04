#include "shell.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "i2cmain.h"


void shell(void)
{
  printf("Starting comandline\n");

  uint32_t exit = 1;
  int32_t len = 0;
  char input[64];

  while(exit)
  {
    do
    {
      input[len] = uartGetc();
      uartPutc(input[len]);
      if(input[len] == 0x7F){
        input[len] = 0;
        len -=2;
      }
    } while(input[len++] != '\n');
    input[len] = 0;
    if (strcmp(input,"exit\n") == 0){exit = 0;}
    else if(strcmp(input,"restart\n") == 0){ printf("restarting...\n");__asm__ ("b _start"); }
    else if(strcmp(input,"shutdown\n")== 0){ printf("shutdown...\n"); uartPuts("\x04\x04\x04");}
    else if(strcmp(input,"i2c\n")== 0){ printf("i2c...\n"); i2cmain();} 
    else
    {
      printf("this command is not implemented yet\n");
    }
    //new commands for the shell should be inserted here
    len=0;


  }
  printf("Close comandline");
}

