#include "mmio.h"
#include "timer.h"
#include "uart.h"

	// The GPIO registers base address.
#define	GPIO_BASE  0x20200000

	// The offsets for reach register.

	// Controls actuation of pull up/down to ALL GPIO pins.
#define	GPPUD  GPIO_BASE + 0x94

	// Controls actuation of pull up/down for specific GPIO pin.
#define	GPPUDCLK0 GPIO_BASE + 0x98

	// The base address for UART.
#define	UART0_BASE  0x20201000

	// The offsets for reach register for the UART.
#define	UART0_DR     UART0_BASE + 0x00
#define	UART0_RSRECR UART0_BASE + 0x04
#define	UART0_FR     UART0_BASE + 0x18
#define	UART0_ILPR   UART0_BASE + 0x20
#define	UART0_IBRD   UART0_BASE + 0x24
#define	UART0_FBRD   UART0_BASE + 0x28
#define	UART0_LCRH   UART0_BASE + 0x2C
#define	UART0_CR     UART0_BASE + 0x30
#define	UART0_IFLS   UART0_BASE + 0x34
#define	UART0_IMSC   UART0_BASE + 0x38
#define	UART0_RIS    UART0_BASE + 0x3C
#define	UART0_MIS    UART0_BASE + 0x40
#define	UART0_ICR    UART0_BASE + 0x44
#define	UART0_DMACR  UART0_BASE + 0x48
#define	UART0_ITCR   UART0_BASE + 0x80
#define	UART0_ITIP   UART0_BASE + 0x84
#define	UART0_ITOP   UART0_BASE + 0x88
#define	UART0_TDR    UART0_BASE + 0x8C


void uartInit(void) {

	// Disable UART0.
	mmioWrite(UART0_CR, 0x00000000);
	// Setup the GPIO pin 14 && 15.
    
	// Disable pull up/down for all GPIO pins & delay for 150 cycles.
	mmioWrite(GPPUD, 0x00000000);
	delay(150);

	// Disable pull up/down for pin 14,15 & delay for 150 cycles.
	mmioWrite(GPPUDCLK0, (1 << 14) | (1 << 15));
	delay(150);

	// Write 0 to GPPUDCLK0 to make it take effect.
	mmioWrite(GPPUDCLK0, 0x00000000);
    
	// Clear pending interrupts.
	mmioWrite(UART0_ICR, 0x7FF);

	// Set integer & fractional part of baud rate.
	// Divider = UART_CLOCK/(16 * Baud)
	// Fraction part register = (Fractional part * 64) + 0.5
	// UART_CLOCK = 3000000; Baud = 115200.

	// Divider = 3000000/(16 * 115200) = 1.627 = ~1.
	// Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
	mmioWrite(UART0_IBRD, 1);
	mmioWrite(UART0_FBRD, 40);

	// Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
	mmioWrite(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

	// Mask all interrupts.
	mmioWrite(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) |
		    (1 << 6) | (1 << 7) | (1 << 8) |
		    (1 << 9) | (1 << 10));

	// Enable UART0, receive & transfer part of UART.
	mmioWrite(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

/*
 * Transmit a byte via UART0.
 * uint8_t Byte: byte to send.
 */
void uartPutc(uint8_t byte) 
{
	// wait for UART to become ready to transmit
	while(1) 
  {
	    if (!(mmioRead(UART0_FR) & (1 << 5))) 
      {
    		break;
      }
  }
	mmioWrite(UART0_DR, byte);
}

/*
 * Receive a byte via UART0.
 *
 * Returns:
 * uint8_t: byte received.
 */
uint8_t uartGetc(void) 
{
	// wait for UART to have recieved something
	while(1) 
  {
    if (!(mmioRead(UART0_FR) & (1 << 4))) 
    {
  		break;
    }
	}
	return mmioRead(UART0_DR);
}

/*
 * print a string to the UART one character at a time
 * const char *str: 0-terminated string
 */
void uartPuts(const char *str) 
{
	while(*str) 
  {
    uartPutc(*str++);
	}
}


