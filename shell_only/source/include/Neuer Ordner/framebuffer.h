#include <stdint.h>

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

typedef struct {
  union {
    uint16_t rgb;

    struct {
      uint16_t b : 5;
      uint16_t g : 6;
      uint16_t r : 5;
    };
  };
} __attribute__ ((__packed__)) rgb16;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} __attribute__ ((__packed__)) rgb24;

typedef struct {
  union {
    uint32_t rgba;
    uint32_t rgb : 24;

    struct {
      uint8_t r;
      uint8_t g;
      uint8_t b;
      uint8_t a;
    };
  };
} __attribute__ ((__packed__)) rgb32;

typedef struct {
  union {
    uint32_t rgb_;

    rgb16 rgb_16;
    rgb24 rgb_24;
    rgb32 rgb_32;
  };
} __attribute__ ((__packed__)) rgb;


extern void fbInitNativ(void);
extern void fbInit(uint32_t set_fb_x, uint32_t set_fb_y, uint32_t set_color_depth);
extern void consoleWriteChar(uint8_t ch);
extern void consoleForegroundColor(rgb new_fg_color);
extern void consoleBackgroundColor(rgb new_bg_color);


#define GPU_MAILBOX_CHANNEL 8

#define COLORMODE_16BIT 2
#define COLORMODE_24BIT 3
#define COLORMODE_32BIT 4

/* 16bit color definitions */
#define COLOR16_RED     0b1111100000000000
#define COLOR16_GREEN   0b0000011111100000
#define COLOR16_BLUE    0b0000000000011111
#define COLOR16_YELLOW  0b1111111111100000
#define COLOR16_MAGENTA 0b1111100000011111
#define COLOR16_CYAN    0b0000011111111111
#define COLOR16_WHITE   0b1111111111111111
#define COLOR16_BLACK   0b0000000000000000

/* 24/32bit color definitions */
#define COLOR32_RED     0xff0000ff
#define COLOR32_GREEN   0xff00ff00
#define COLOR32_BLUE    0xffff0000
#define COLOR32_YELLOW  0xff00ffff
#define COLOR32_MAGENTA 0xffff00ff
#define COLOR32_CYAN    0xffffff00
#define COLOR32_WHITE   0xffffffff
#define COLOR32_BLACK   0x00000000


#endif	/* FRAMEBUFFER_H */
