#include "timer.h"


/*
 * delay function
 * int32_t delay: number of cycles to delay
 *
 * This just loops <delay> times in a way that the compiler
 * wont optimize away.
 */
void delay(uint32_t count) 
{
  asm volatile("1: subs %[count], %[count], #1; bne 1b": : [count]"r"(count));
}
