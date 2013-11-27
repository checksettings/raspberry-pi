#include "shell.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"

typedef struct 
{
  fcn_ptr command;
  char command_name[24];
}command_struct;

command_struct allcommands[MAXNUMBEROFFUNCTIONS];
int commandcounter = 0;

void helpOutput();

void shell(void)
{

  addNewCommand(helpOutput,"help\n");
  printf("Starting Shell!!\n");

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
      while(counter < MAXNUMBEROFFUNCTIONS){
        if(strcmp(input,allcommands[counter].command_name)==0)
        {
          allcommands[counter].command();
          break;
        }
        counter++;
      }
    }
    len=0;
    counter =0;

  }
  printf("Close shell");
}

void addNewCommand(fcn_ptr function_pointer, char command_name[24])
{
  allcommands[commandcounter].command = function_pointer;
  int i;
  for( i= 0; i < 24 ;i++)
  {
    allcommands[commandcounter].command_name[i] = command_name[i];
  }
  commandcounter++;
}

void helpOutput()
{
  printf("-----------------------------------------\n");
  printf("Helpfunction\n");
  printf("The following Commands are availabel now!\n");
  
} 

