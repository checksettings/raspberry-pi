#include <stdint.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

extern void fbInitNativ(void);
extern void fbInit(uint32_t set_fb_x, uint32_t set_fb_y);
extern void consoleWriteChar(uint8_t ch);
extern void consoleForegroundColor(uint16_t color);
extern void consoleBackgroundColor(uint16_t color);

#define GPU_MAILBOX_CHANNEL 8

/* Control characters for the console */
#define COLOR16_RED     0b1111100000000000
#define COLOR16_GREEN   0b0000011111100000
#define COLOR16_BLUE    0b0000000000011111
#define COLOR16_YELLOW  0b1111111111100000
#define COLOR16_MAGENTA 0b1111100000011111
#define COLOR16_CYAN    0b0000011111111111
#define COLOR16_WHITE   0b1111111111111111
#define COLOR16_BLACK   0b0000000000000000


#endif	/* FRAMEBUFFER_H */
