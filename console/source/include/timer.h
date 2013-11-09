#ifndef TIMER_H
#define TIMER_H
/*
 * delay function
 * int32_t delay: number of cycles to delay
 *
 * This just loops <delay> times in a way that the compiler
 * wont optimize away.
 */
void delay(int32_t count) 
{
	asm volatile("1: subs %[count], %[count], #1; bne 1b": : [count]"r"(count));
}

#endif // #ifndef TIMER_H
