#include "framebuffer.h"


#define GPIO_FSEL_ADDR 0x20200000

#define GPFSEL1_OFFS   4
#define LED_BIT_SET    18

#define GPSET0_OFFS    28
#define GPCLR0_OFFS    40
#define GPIO_PIN_OFFS  16

#define WAIT_DELAY      0x3F0000

#define mmio32(x) (*(volatile uint32_t*) (x))


void delay_with_loop(uint32_t value);

void main()
{
  uint32_t fb_x = 640;
  uint32_t fb_y = 480;
  fb_init(fb_x, fb_y);
  //~ fb_init_nativ();

  console_write("This is my first Output on console!!");

  // set as output
  mmio32(GPIO_FSEL_ADDR + GPFSEL1_OFFS) |= (1<<LED_BIT_SET);

  while(1)
  {
    // switch on led
    mmio32(GPIO_FSEL_ADDR + GPCLR0_OFFS) |= (1<<GPIO_PIN_OFFS);

    // wait 1
    delay_with_loop(WAIT_DELAY);

    // switch off led
    mmio32(GPIO_FSEL_ADDR + GPSET0_OFFS) |= (1<<GPIO_PIN_OFFS);

    // wait 2
    delay_with_loop(WAIT_DELAY);
  }
}

void delay_with_loop(volatile uint32_t value)
{
  for(;  value != 0; value--);
}
