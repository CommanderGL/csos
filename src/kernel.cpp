#include "term.h"

extern "C" {

#include "catppuccin.h"
#include "graphics.h"
#include "interupts/exceptions.h"
#include "interupts/idt.h"
#include "interupts/ints.h"
#include "interupts/pic.h"
#include "io.h"
#include "itoa.h"
#include "keyboard/keyboard.h"
#include "multiboot.h"
#include "string.h"
#include <stdbool.h>

#if defined(__linux__)
#error \
    "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

multiboot_info_t* mbi;

uint32_t fb_addr[SCREEN_WIDTH * SCREEN_HEIGHT + 1];
struct Framebuffer fb;

void keyboard_handler_c() {
  while ((inportb(0x3f8 + 5) & 0x20) == 0) {}
  outportb(0x3f8, 'H');
  while ((inportb(0x3f8 + 5) & 0x20) == 0) {}
  outportb(0x3f8, 'i');

  inportb(0x60);
  print2("KEY!", 100, 50, CATPPUCCIN_YELLOW, CATPPUCCIN_BASE, fb);
  memcpy((uint32_t*)mbi->framebuffer_addr, fb_addr, sizeof(fb_addr));

  outportb(0x20, 0x20);
}

void kernel_main(unsigned long magic, unsigned long addr) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) panic("Invalid Magic Number");
  mbi = (multiboot_info_t*)addr;

  asm("cli");
  remapPIC();
  maskIRQ(ALL);
  unmaskIRQ(KEYBOARD);
  loadExceptions();
  addInt(33, keyboard_handler, 0);
  // keyboard_init(kbm_poll);
  loadIDTR();
  asm("sti");

  fb.addr = fb_addr;
  fb.width = SCREEN_WIDTH;
  fb.height = SCREEN_HEIGHT;
  fb.pitch = mbi->framebuffer_pitch;

  clr_screen(CATPPUCCIN_BASE, fb);

  drawLine(fb.width, 0, 0, fb.height, CATPPUCCIN_RED, fb);
  drawRectOutline(fb.width - 100 - 10, 10, 100, 50, CATPPUCCIN_SURFACE1, fb);
  drawRectFill(fb.width - 50 - 10, 10 + 50 + 10, 50, 100, CATPPUCCIN_GREEN, fb);
  print2("Hello, World!", fb.width - 10 - 13 * CHAR_SIZE_2, fb.height - 10 - CHAR_SIZE_2, CATPPUCCIN_TEXT, CATPPUCCIN_BASE, fb);
  print("Hello, World!", fb.width - 10 - 13 * CHAR_SIZE, fb.height - 10 - CHAR_SIZE_2 - 10 - CHAR_SIZE, CATPPUCCIN_TEXT, CATPPUCCIN_BASE, fb);

  char str[4];
  print2(itoa(mbi->framebuffer_width, str, 10), 0, 500, CATPPUCCIN_YELLOW, CATPPUCCIN_BASE, fb);
  print2(itoa(mbi->framebuffer_height, str, 10), 0, 500 + CHAR_SIZE_2, CATPPUCCIN_YELLOW, CATPPUCCIN_BASE, fb);

  Term term("", 5, 5, fb, {}, true);
  term.putChar('A', CATPPUCCIN_TEXT, CATPPUCCIN_BASE);
  term.putChar('b', CATPPUCCIN_TEXT, CATPPUCCIN_BASE);
  term.putChar('C', CATPPUCCIN_TEXT, CATPPUCCIN_BASE);

  memcpy((uint32_t*)mbi->framebuffer_addr, fb_addr, sizeof(fb_addr));

  while ((inportb(0x3f8 + 5) & 0x20) == 0) {}
  outportb(0x3f8, 'H');
  while ((inportb(0x3f8 + 5) & 0x20) == 0) {}
  outportb(0x3f8, 'i');

  while (true) {}
}
}
