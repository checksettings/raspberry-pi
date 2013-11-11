#include <stdint.h>

#ifndef STDIO_H
#define STDIO_H

#define EOF -1

extern int32_t putchar(int32_t ch);
extern int32_t puts(const char *output_string);
extern int32_t atoi(const char *string);
extern int printf(const char *format, ...);

#endif /* STDIO_H */
