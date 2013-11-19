#include <stdint.h>

#ifndef MEMUTILS_H
#define MEMUTILS_H

/* Clear length bytes of memory (set to 0) starting at address */
extern void memclr(void *address, uint32_t length);

/* Move length bytes from src to dest. Memory areas may overlap */
extern void *memmove(void *dest, const void *src, uint32_t length);

#endif	/* MEMUTILS_H */
