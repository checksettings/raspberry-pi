#include <stdint.h>
#include "framebuffer.h"
#include "stdio.h"
#include "stdlib.h"
#include "timer.h"
#include "mmio.h"
#include "uart.h"
#include "string.h"
#include "shell.h"


void otherprogramm(void)
{
  printf("now executing myotherprogrammmain\n");
}

void newcommandstoadd() {
  addNewCommand(otherprogramm,"startmymain",NULL);
}
