#include "term.h"

char* strcpy(char* strDest, const char* strSrc) {
  while ((*strDest++ = *strSrc++) != '\0') {}
  return strDest;
}

Term::Term(const char* prompt, int xMargin, int yMargin, struct Framebuffer fb, char data[TERM_WIDTH * TERM_HEIGHT], bool doubleSize) {
  this->prompt = prompt;
  this->xMargin = xMargin;
  this->yMargin = yMargin;
  this->fb = fb;
  strcpy(this->data, data);
  this->doubleSize = doubleSize;
}

void Term::putChar(unsigned char ch, uint32_t fg, uint32_t bg) {
  if (ch == '\n') {
    xOffset = 0;
    yOffset += doubleSize ? CHAR_SIZE_2 : CHAR_SIZE;
    return;
  }

  if (doubleSize) {
    putChar2(ch, xMargin + xOffset, yMargin + yOffset, fg, bg, fb);
    xOffset += CHAR_SIZE_2;
    return;
  }
  ::putChar(ch, xMargin + xOffset, yMargin + yOffset, fg, bg, fb);
  xOffset += CHAR_SIZE;
}
