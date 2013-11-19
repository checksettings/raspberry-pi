#include <stdint.h>

#ifndef STDIO_H
#define STDIO_H

#define EOF -1

extern int32_t putchar(int32_t ch);
extern int32_t puts(const char *output_string);
extern int32_t atoi(const char *string);
extern int32_t printf(const char *format, ...);
extern int32_t getStdOutput();
extern void setStdOutput(int32_t std_out);

#endif /* STDIO_H */
