/* mmio.h - access to MMIO registers */
/* Copyright (C) 2013 Goswin von Brederlow <goswin-v-b@web.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include <stdint.h>
#ifndef MMIO_H
#define MMIO_H

// The GPIO registers base address.
#define	GPIO_BASE  0x20200000

// The offsets for reach register.

// Controls actuation of pull up/down to ALL GPIO pins.
#define	GPPUD  GPIO_BASE + 0x94

// Controls actuation of pull up/down for specific GPIO pin.
#define	GPPUDCLK0 GPIO_BASE + 0x98

// Parameters for GPIO with ACT-LED on raspberry pi
#define GPFSEL1_OFFS   4
#define LED_BIT_SET    18

#define GPSET0_OFFS    28
#define GPCLR0_OFFS    40
#define GPIO_PIN_OFFS  16

#define mmio32(x) (*(volatile uint32_t*) (GPIO_BASE + x))


    // write to MMIO register
  static inline void mmioWrite(uint32_t reg, uint32_t data) 
  {
  	uint32_t *ptr = (uint32_t*)reg;
  	asm volatile("str %[data], [%[reg]]"
		     : : [reg]"r"(ptr), [data]"r"(data));
  }

    // read from MMIO register
  static inline uint32_t mmioRead(uint32_t reg) 
  {
  	uint32_t *ptr = (uint32_t*)reg;
  	uint32_t data;
  	asm volatile("ldr %[data], [%[reg]]"
		     : [data]"=r"(data) : [reg]"r"(ptr));
  	return data;
  }


#endif // #ifndef MMIO_H
