#include "mouse.h"
#include "../interupts/idt.h"
#include "../interupts/ints.h"
#include "../io.h"
#include "../string.h"

extern struct Framebuffer direct_fb;

byte mouse_cycle = 2;
char mouse_byte[3];
int mouse_x = 10;
int mouse_y = SCREEN_HEIGHT - 10;
int old_mouse_x;
int old_mouse_y;

const uint32_t cursor_sprite[CURSOR_HEIGHT][CURSOR_WIDTH] = {
  {0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT},
  {
    0xffffff,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0x000000,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
    0xffffff,
  },
  {0xffffff,
   0x000000,
   0x000000,
   0x000000,
   0xffffff,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   0xffffff,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   0xffffff,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
  {0xffffff,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   0xffffff,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT},
  {TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   0xffffff,
   0x000000,
   0x000000,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT},
  {TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT,
   0xffffff,
   0xffffff,
   TRANSPARENT,
   TRANSPARENT,
   TRANSPARENT},
};

void draw_cursor() {
  drawRectFill(old_mouse_x, SCREEN_HEIGHT - old_mouse_y, CURSOR_WIDTH, CURSOR_HEIGHT, CATPPUCCIN_BASE, direct_fb); // !TODO - Replace with old data instead of "CATPPUCCIN_BASE"

  for (int i = 0; i < CURSOR_HEIGHT - 1; i++) {
    for (int j = 0; j < CURSOR_WIDTH; j++) {
      putPixel(mouse_x + j, SCREEN_HEIGHT - mouse_y + i, cursor_sprite[i][j], direct_fb);
    }
  }
}

void mouse_handler_c() {
  switch (mouse_cycle) {
  case 0:
    mouse_byte[0] = inportb(0x60);
    mouse_cycle++;
    break;
  case 1:
    mouse_byte[1] = inportb(0x60);
    old_mouse_x = mouse_x;
    mouse_x += mouse_byte[1];
    if (mouse_x + CURSOR_WIDTH > SCREEN_WIDTH)
      mouse_x = SCREEN_WIDTH - CURSOR_WIDTH;
    else if (mouse_x < 0)
      mouse_x = 0;
    mouse_cycle++;
    break;
  case 2:
    mouse_byte[2] = inportb(0x60);
    old_mouse_y = mouse_y;
    mouse_y += mouse_byte[2];
    if (SCREEN_HEIGHT - mouse_y + CURSOR_HEIGHT > SCREEN_HEIGHT)
      mouse_y = CURSOR_HEIGHT;
    else if (SCREEN_HEIGHT - mouse_y < 0)
      mouse_y = SCREEN_HEIGHT;

    draw_cursor();

    mouse_cycle = 0;
    break;
  }

  outportb(0xa0, 0x20);
  outportb(0x20, 0x20);
}

inline void mouse_wait(byte type) {
  dword time_out = 100000;
  if (type == 0) {
    while (time_out--) {
      if ((inportb(0x64) & 1) == 1) {
        return;
      }
    }
    return;
  } else {
    while (time_out--) {
      if ((inportb(0x64) & 2) == 0) {
        return;
      }
    }
    return;
  }
}

inline void mouse_write(byte data) {
  mouse_wait(1);
  outportb(0x64, 0xD4);
  mouse_wait(1);
  outportb(0x60, data);
}

byte mouse_read() {
  mouse_wait(0);
  return inportb(0x60);
}

void mouse_init() {
  byte status;

  mouse_wait(1);
  outportb(0x64, 0xa8);

  mouse_wait(1);
  outportb(0x64, 0x20);
  mouse_wait(0);
  status = inportb(0x60) | 2;
  mouse_wait(1);
  outportb(0x64, 0x60);
  mouse_wait(1);
  outportb(0x60, status);

  mouse_write(0xf6);
  mouse_read();

  mouse_write(0xf4);
  mouse_read();

  addInt(44, mouse_handler, 0);
}
