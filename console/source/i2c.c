#include "i2c.h"
#include "stdio.h"

// By default I2C code is generated for the V2 RPi 
// which has SDA1 and SCL1 connected.
#define I2C_V1

#ifdef I2C_V1
  #define BCM2835_BSC_BASE BCM2835_BSC0_BASE
#else
  #define BCM2835_BSC_BASE BCM2835_BSC1_BASE
#endif 

// I2C The time needed to transmit one byte. In microseconds.
static uint32_t i2c_byte_wait_us = 0;

void delay_with_loop(volatile uint32_t value)
{
	for(;  value != 0; value--);
}

void initLED()
{
  mmio32_2(GPIO_FSEL_ADDR + GPFSEL1_OFFS) |= (GPIO_OUTPUT<<GPIO_PIN_16);
}

void setLEDOn()
{
  mmio32_2(GPIO_FSEL_ADDR + GPCLR0_OFFS) |= (1<<GPIO_PIN_OFFS);
}

void setLEDOff()
{
  mmio32_2(GPIO_FSEL_ADDR + GPSET0_OFFS) |= (1<<GPIO_PIN_OFFS);
}


void i2cInit(void)
{
  uint16_t div;
	initLED();
  setLEDOn();

  // set Pins
  #ifdef I2C_V1
    printf("i2c pin V1 set\n");
    mmio32_2(GPIO_FSEL_ADDR + GPFSEL0_OFFS) |= (GPIO_ALTERNATIVE0<<GPIO_PIN_0);
    mmio32_2(GPIO_FSEL_ADDR + GPFSEL0_OFFS) |= (GPIO_ALTERNATIVE0<<GPIO_PIN_1);
  #else
    // 48024  --  100 1000 0000 0010 0100
    // 48924  --  100 1000 1001 0010 0100
    // 8 und 11 auf 1
    printf("i2c pin V2 set\n");
    mmio32_2(GPIO_FSEL_ADDR + GPFSEL0_OFFS) |= (GPIO_ALTERNATIVE0<<GPIO_PIN_2);
    mmio32_2(GPIO_FSEL_ADDR + GPFSEL0_OFFS) |= (GPIO_ALTERNATIVE0<<GPIO_PIN_3);
  #endif    

  // readout devider
  div = mmio32_2(BCM2835_BSC_BASE+BCM2835_BSC_DIV);
  printf("i2c div: %x\n",div);
  i2c_byte_wait_us = (div/250)*9;

}


void i2cClose(void)
{
  #ifdef I2C_V1
    mmio32_2(GPIO_FSEL_ADDR + GPFSEL0_OFFS) |= (GPIO_INPUT<<GPIO_PIN_0);
    mmio32_2(GPIO_FSEL_ADDR + GPFSEL0_OFFS) |= (GPIO_INPUT<<GPIO_PIN_1);
  #else
    mmio32_2(GPIO_FSEL_ADDR + GPFSEL0_OFFS) |= (GPIO_INPUT<<GPIO_PIN_2);
    mmio32_2(GPIO_FSEL_ADDR + GPFSEL0_OFFS) |= (GPIO_INPUT<<GPIO_PIN_3);
  #endif 
  
  delay_with_loop(WAIT_DELAY_2);
  setLEDOff();
}


uint8_t i2cRead(uint8_t address, uint32_t bytes, char *data)
{
  uint32_t remaining = bytes;
  uint32_t i = 0;
  uint8_t reason = BCM2835_I2C_REASON_OK;

  // Set Address
  mmio32_2(BCM2835_BSC_BASE+BCM2835_BSC_A) = (address >> 1);

  // Clear FIFO
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_C_CLEAR_1);

  // Clear Status
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) = (BCM2835_BSC_S_CLKT | 
                                              BCM2835_BSC_S_ERR  | 
                                              BCM2835_BSC_S_DONE);
  // Set Data Length
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_DLEN) = (bytes);

  // Start read
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_C_I2CEN | 
                                              BCM2835_BSC_C_ST    |
                                              BCM2835_BSC_C_READ);
  // wait for transfer to complete
  while (!((mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S)) & BCM2835_BSC_S_DONE))
  {
    // we must empty the FIFO as it is populated and not use any delay
    while (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) & BCM2835_BSC_S_RXD)
  	{
      // Read from FIFO
      data[i] = mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO);
      i++;
      remaining--;
  	}
  }

  // transfer has finished - grab any remaining stuff in FIFO
  while (remaining && (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) & BCM2835_BSC_S_RXD))
  {
    // Read from FIFO
    data[i] = mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO);
    i++;
    remaining--;
  }

  // Received a NACK
  if (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO) & BCM2835_BSC_S_ERR)
  {
    reason = BCM2835_I2C_REASON_ERROR_NACK;
  }

  // Received Clock Stretch Timeout
  else if (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO) & BCM2835_BSC_S_CLKT)
  {
    reason = BCM2835_I2C_REASON_ERROR_CLKT;
  }

  // Not all data is received
  else if (remaining)
  {
    reason = BCM2835_I2C_REASON_ERROR_DATA;
  }

  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_S_DONE);

  return reason;
}


uint8_t i2cReadWithRegister(uint8_t address, const char *reg_address, uint32_t bytes, char *data)
{
  uint32_t remaining = bytes;
  uint32_t i = 0;
  uint8_t reason = BCM2835_I2C_REASON_OK;

  // Set Address
  mmio32_2(BCM2835_BSC_BASE+BCM2835_BSC_A) = (address >> 1);

  // Clear FIFO
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_C_CLEAR_1);

  // Clear Status
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) = (BCM2835_BSC_S_CLKT | 
                                                BCM2835_BSC_S_ERR  | 
                                                BCM2835_BSC_S_DONE);
  // Set Data Length
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_DLEN) = 1;

  // Enable device and start transfer
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_C_I2CEN);
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO) = (reg_address[0]);
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_C_I2CEN | 
                                                BCM2835_BSC_C_ST);

  // poll for transfer has started
  while ( !((mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S)) & BCM2835_BSC_S_TA ) )
  {
    if((mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S)) & BCM2835_BSC_S_DONE)
      break;
  }

  // Set Data Length
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_DLEN) = bytes;

  // Start read
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_C_I2CEN | 
                                                BCM2835_BSC_C_ST    |
                                                BCM2835_BSC_C_READ);
  
  // Wait for write to complete and first byte back
  delay_with_loop(0xFFFF);

  // wait for transfer to complete
  while (!((mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S)) & BCM2835_BSC_S_DONE))
  {
    // we must empty the FIFO as it is populated and not use any delay
    while (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) & BCM2835_BSC_S_RXD)
  	{
      // Read from FIFO
      data[i] = mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO);
      i++;
      remaining--;
  	}
  }

  // transfer has finished - grab any remaining stuff in FIFO
  while (remaining && (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) & BCM2835_BSC_S_RXD))
  {
    // Read from FIFO
    data[i] = mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO);
    i++;
    remaining--;
  }

  // Received a NACK
  if (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO) & BCM2835_BSC_S_ERR)
  {
    reason = BCM2835_I2C_REASON_ERROR_NACK;
  }

  // Received Clock Stretch Timeout
  else if (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO) & BCM2835_BSC_S_CLKT)
  {
    reason = BCM2835_I2C_REASON_ERROR_CLKT;
  }

  // Not all data is received
  else if (remaining)
  {
    reason = BCM2835_I2C_REASON_ERROR_DATA;
  }

  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_S_DONE);

  return reason;
}


uint8_t i2cWrite(uint8_t address, uint32_t bytes, const char *data)
{
  uint32_t remaining = bytes;
  uint32_t i = 0;
  uint8_t reason = BCM2835_I2C_REASON_OK;

  // Set Address
  mmio32_2(BCM2835_BSC_BASE+BCM2835_BSC_A) = (address >> 1);

  // Clear FIFO
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_C_CLEAR_1);

  // Clear Status
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) = (BCM2835_BSC_S_CLKT | 
                                                  BCM2835_BSC_S_ERR  | 
                                                  BCM2835_BSC_S_DONE);
  // Set Data Length
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_DLEN) = bytes;

  // Fill FIFO
  while( remaining && ( i < BCM2835_BSC_FIFO_SIZE ) )
  {
    mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO) = (data[i]);
    i++;
    remaining--;
  }

  // Enable device and start transfer
  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = ( BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);

  // Transfer is over when BCM2835_BSC_S_DONE
  while(!(mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) & BCM2835_BSC_S_DONE ))
  {
    while ( remaining && (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) & BCM2835_BSC_S_TXD ))
  	{
      // Write to FIFO
      mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_FIFO) = ( data[i]);
      i++;
      remaining--;
  	}
  }

  if (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) & BCM2835_BSC_S_ERR)
  {
	  reason = BCM2835_I2C_REASON_ERROR_NACK;
  }
  // Received Clock Stretch Timeout
  else if (mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_S) & BCM2835_BSC_S_CLKT)
  {
	  reason = BCM2835_I2C_REASON_ERROR_CLKT;
  }
  // Not all data is sent
  else if (remaining)
  {
	  reason = BCM2835_I2C_REASON_ERROR_DATA;
  }

  mmio32_2(BCM2835_BSC_BASE + BCM2835_BSC_C) = (BCM2835_BSC_S_DONE);
  return reason;
}

