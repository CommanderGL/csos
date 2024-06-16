#include <stdint.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

struct Framebuffer {
  uint32_t* addr;
  int width;
  int height;
  int pitch;
};

#define CHAR_SIZE 8
#define CHAR_SIZE_2 16

// #define putPixel(x, y, color) ((uint32_t*)mbi->framebuffer_addr)[(y) * (mbi->framebuffer_pitch / 4) + (x)] = color
__inline__ void putPixel(int x, int y, uint32_t color, struct Framebuffer fb);
void drawLine(int x0, int y0, int x1, int y1, uint32_t color, struct Framebuffer fb);
void drawRectOutline(int x, int y, int w, int h, uint32_t color, struct Framebuffer fb);
void drawRectFill(int x, int y, int w, int h, uint32_t color, struct Framebuffer fb);
void putChar(unsigned char ch, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb);
void putChar2(unsigned char ch, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb);
void print(const char* data, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb);
void print2(const char* data, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb);
void clr_screen(uint32_t color, struct Framebuffer fb);
void update_framebuffer(uint32_t* dest, uint32_t* src);

#endif
