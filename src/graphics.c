#include "graphics.h"
#include "font8x8_basic.h"
#include <stddef.h>
#include <stdint.h>

int abs(int num) {
  return num < 0 ? -num : num;
}

__inline__ void putPixel(int x, int y, uint32_t color, struct Framebuffer fb) {
  fb.addr[(y) * (fb.pitch / 4) + (x)] = color;
}

void drawLine(int x0, int y0, int x1, int y1, uint32_t color, struct Framebuffer fb) {
  int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
  int err = (dx > dy ? dx : -dy) / 2, e2;

  for (;;) {
    putPixel(x0, y0, color, fb);
    if (x0 == x1 && y0 == y1) break;
    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
}

void drawRectOutline(int x, int y, int w, int h, uint32_t color, struct Framebuffer fb) {
  drawLine(x, y, x + w, y, color, fb);
  drawLine(x + w, y, x + w, y + h, color, fb);
  drawLine(x + w, y + h, x, y + h, color, fb);
  drawLine(x, y + h, x, y, color, fb);
}

void drawRectFill(int x, int y, int w, int h, uint32_t color, struct Framebuffer fb) {
  for (int i = x; i < x + w; i++) {
    for (int j = y; j < y + h; j++) {
      putPixel(i, j, color, fb);
    }
  }
}

void putChar(unsigned char ch, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      putPixel(x + i, y + j, font8x8_basic[ch][j] & 1 << i ? fg : bg, fb);
    }
  }
}

void putChar2(unsigned char ch, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb) {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      putPixel(x + i * 2, y + j * 2, font8x8_basic[ch][j] & 1 << i ? fg : bg, fb);
      putPixel(x + i * 2 + 1, y + j * 2, font8x8_basic[ch][j] & 1 << i ? fg : bg, fb);
      putPixel(x + i * 2, y + j * 2 + 1, font8x8_basic[ch][j] & 1 << i ? fg : bg, fb);
      putPixel(x + i * 2 + 1, y + j * 2 + 1, font8x8_basic[ch][j] & 1 << i ? fg : bg, fb);
    }
  }
}

void print(const char* data, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb) {
  int yOffset = 0;
  for (size_t i = 0; i < strlen(data); i++) {
    if (data[i] == '\n') {
      yOffset += CHAR_SIZE;
      continue;
    }
    putChar(data[i], x + i * CHAR_SIZE, y + yOffset, fg, bg, fb);
  }
}

void print2(const char* data, int x, int y, uint32_t fg, uint32_t bg, struct Framebuffer fb) {
  int yOffset = 0;
  for (size_t i = 0; i < strlen(data); i++) {
    if (data[i] == '\n') {
      yOffset += CHAR_SIZE_2;
      continue;
    }
    putChar2(data[i], x + i * CHAR_SIZE_2, y + yOffset, fg, bg, fb);
  }
}

void clr_screen(uint32_t color, struct Framebuffer fb) {
  for (int i = 0; i < fb.width; i++) {
    for (int j = 0; j < fb.height; j++) {
      putPixel(i, j, color, fb);
    }
  }
}
