#include <stdint.h>

#ifndef GRAPHICS_H
#define GRAPHICS_H

struct Framebuffer {
  uint32_t* addr;
  int width;
  int height;
  int pitch;
};

#define TRANSPARENT 0xee66ff

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900

#define CHAR_SIZE 8
#define CHAR_SIZE_2 16

inline void putPixel(int x, int y, uint32_t color, struct Framebuffer fb) {
  if (color != TRANSPARENT) fb.addr[(y) * (fb.pitch / 4) + (x)] = color;
}

void drawLine(int x0, int y0, int x1, int y1, uint32_t color, struct Framebuffer fb);
void drawRectOutline(int x, int y, int w, int h, uint32_t color, struct Framebuffer fb);
void drawRectFill(int x, int y, int w, int h, uint32_t color, struct Framebuffer fb);
void putChar(unsigned char ch, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb);
void putChar2(unsigned char ch, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb);
void print(const char* data, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb);
void print2(const char* data, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb);
void clr_screen(uint32_t color, struct Framebuffer fb);

#endif
