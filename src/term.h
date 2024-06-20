#include "graphics/graphics.h"
#include <stdbool.h>

#ifndef TERM_H
#define TERM_H

void term_init(const char* prompt, int xMargin, int yMargin, int width, int height, bool doubleSize, struct Framebuffer fb);
void term_putChar(unsigned char ch, uint32_t fg, uint32_t bg);

#endif
