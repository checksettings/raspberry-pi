#include <stdint.h>
#include "mailbox.h"

#define GPIO_FSEL_ADDR 0x20200000

#define GPFSEL1_OFFS   4
#define LED_BIT_SET    18

#define GPSET0_OFFS    28
#define GPCLR0_OFFS    40
#define GPIO_PIN_OFFS  16

#define WAIT_DELAY      0x3F0000

#define mmio32(x) (*(volatile uint32_t*) (x))


/* Static memory areas */
/* Memory Array for the Mailbox */
static uint32_t* mailbox = (uint32_t* const) 0x9000;


void delay_with_loop(uint32_t value);

void main()
{
  // set as output
  mmio32(GPIO_FSEL_ADDR + GPFSEL1_OFFS) |= (1<<LED_BIT_SET);

  mailbox[0] = 1024;
  mailbox[1] = 768;
  mailbox[2] = 1024;
  mailbox[3] = 768;
  mailbox[4] = 0;
  mailbox[5] = 16;
  mailbox[6] = 0;
  mailbox[7] = 0;
  mailbox[8] = 0;
  mailbox[9] = 0;

  mbox_write((uint32_t)mailbox);
  if(mbox_read()==0)
  {
    uint16_t* start = (uint16_t*)(mailbox[8]);
    if(start == 0)
      mmio32(GPIO_FSEL_ADDR + GPCLR0_OFFS) |= (1<<GPIO_PIN_OFFS);  // switch on led
    uint16_t color = 0;
    uint32_t x = 0;
    uint32_t y = 0;
    while(1){
      for(y=0; y < 768; ++y)
      {
        for(x=0; x < 1024; ++x)
        {
          start[y*1024+x] = color;
        }
        ++color;
        if(color > 65000) color = 0;
      }

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
}

void delay_with_loop(volatile uint32_t value)
{
  for(;  value != 0; value--);
}
