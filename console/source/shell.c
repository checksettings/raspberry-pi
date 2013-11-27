#include "shell.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"

void shell(void)
{
  printf("Starting comandline\n");

  //printf("name: %s", commands_name[1]);

  uint32_t exit = 1;
  int32_t len = 0;
  char input[64];
  uint32_t counter = 0;
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
    else
    {
      while(strcmp(commands_name[counter],"\0")!=0){
        //printf("bin in while counter = %d\n", counter);
        commands[counter]();
        counter++;
      }
    }
    //insert new commands here
    len=0;
    counter =0;

  }
  printf("Close shell");
}

