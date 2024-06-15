#include <stdint.h>

extern "C" {
#include "graphics.h"
}

#ifndef TERM_H
#define TERM_H

const int TERM_WIDTH = SCREEN_WIDTH / CHAR_SIZE;
const int TERM_HEIGHT = SCREEN_HEIGHT / CHAR_SIZE;

class Term {
public:
  Term(const char* prompt, int xMargin, int yMargin, struct Framebuffer fb, char data[TERM_HEIGHT * TERM_WIDTH], bool doubleSize);

  void putChar(unsigned char ch, uint32_t fg, uint32_t bg);

private:
  const char* prompt;
  int xOffset = 0;
  int yOffset = 0;
  int xMargin;
  int yMargin;
  struct Framebuffer fb;
  char data[TERM_HEIGHT * TERM_WIDTH];
  bool doubleSize;
};

#endif
