#include "term.h"
#include "io.h"
#include <stdint.h>

int term_width = SCREEN_WIDTH / CHAR_SIZE;
int term_height = SCREEN_HEIGHT / CHAR_SIZE;
const char* term_prompt = "";
int term_xMargin = 0;
int term_yMargin = 0;
bool term_doubleSize = false;
struct Framebuffer term_fb;

int term_column = 0;
int term_row = 0;

void term_init(const char* prompt, int xMargin, int yMargin, int width, int height, bool doubleSize, struct Framebuffer fb) {
  term_prompt = prompt;
  term_xMargin = xMargin;
  term_yMargin = yMargin;
  term_width = (width - xMargin) / (doubleSize ? CHAR_SIZE_2 : CHAR_SIZE);
  term_height = (height - yMargin) / (doubleSize ? CHAR_SIZE_2 : CHAR_SIZE);
  term_doubleSize = doubleSize;
  term_fb = fb;
}

void term_putChar(unsigned char ch, uint32_t fg, uint32_t bg) {
  if (ch == '\0') return;

  if (ch == '\n') {
    term_row++;
    term_column = 0;
    if (term_row == term_height) {
      // !TODO - Add term scrolling.
    }
    return;
  }

  if (ch == '\b') {
    if (term_column == 0) return;
    term_column--;
    drawRectFill(term_xMargin + term_column * (term_doubleSize ? CHAR_SIZE_2 : CHAR_SIZE), term_yMargin + term_row * (term_doubleSize ? CHAR_SIZE_2 : CHAR_SIZE), term_doubleSize ? CHAR_SIZE_2 : CHAR_SIZE, term_doubleSize ? CHAR_SIZE_2 : CHAR_SIZE, bg, term_fb);
    return;
  }

  if (term_doubleSize) {
    putChar2(ch, term_xMargin + term_column * CHAR_SIZE_2, term_yMargin + term_row * CHAR_SIZE_2, fg, bg, term_fb);
  } else {
    putChar(ch, term_xMargin + term_column * CHAR_SIZE, term_yMargin + term_row * CHAR_SIZE, fg, bg, term_fb);
  }

  term_column++;
  if (term_column == term_width) {
    term_column = 0;
    term_row++;
    if (term_row == term_height) {
      // !TODO - Add term scrolling.
    }
  }
}
