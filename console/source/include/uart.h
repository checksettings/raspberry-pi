#ifndef UART_H
#define UART_H

#include "stdint.h"

extern void uartInit(void);
extern void uartPutc(uint8_t byte);
extern uint8_t uartGetc(void);
extern void uartPuts(const char *str);

#endif	/* UART_H */
