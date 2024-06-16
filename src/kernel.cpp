#include "term.h"

extern "C" {

#include "catppuccin.h"
#include "graphics.h"
#include "interupts/exceptions.h"
#include "interupts/idt.h"
#include "interupts/ints.h"
#include "interupts/pic.h"
#include "io.h"
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

uint32_t fb_addr[SCREEN_WIDTH * SCREEN_HEIGHT];
struct Framebuffer fb;

void keyboard_handler_c() {
  outportb(0x20, 0x20);

  unsigned char scan_code = inportb(0x60);
  print2("KEY!", 100, 50, CATPPUCCIN_YELLOW, CATPPUCCIN_BASE, fb);
  memcpy((uint32_t*)mbi->framebuffer_addr, fb_addr, sizeof(fb_addr));
}

void kernel_main(unsigned long magic, unsigned long addr) {
  if (magic != MULTIBOOT_BOOTLOADER_MAGIC) panic("Invalid Magic Number");
  mbi = (multiboot_info_t*)addr;

  asm("cli");
  remapPIC(0x20, 0x28);
  maskIRQ(ALL);
  unmaskIRQ(KEYBOARD);
  loadExceptions();
  addInt(35, keyboard_handler, 0);
  keyboard_init(kbm_poll);
  loadIDTR();
  asm("sti");

  fb.addr = fb_addr;
  fb.width = SCREEN_WIDTH;
  fb.height = SCREEN_HEIGHT;
  fb.pitch = mbi->framebuffer_pitch;

  clr_screen(CATPPUCCIN_BASE, fb);

  drawLine(mbi->framebuffer_width, 0, 0, mbi->framebuffer_height, CATPPUCCIN_RED, fb);
  drawRectOutline(mbi->framebuffer_width - 100 - 10, 10, 100, 50, CATPPUCCIN_SURFACE1, fb);
  drawRectFill(mbi->framebuffer_width - 50 - 10, 10 + 50 + 10, 50, 100, CATPPUCCIN_GREEN, fb);
  print2("Hello, World!", mbi->framebuffer_width - 10 - 13 * CHAR_SIZE_2, mbi->framebuffer_height - 10 - CHAR_SIZE_2, CATPPUCCIN_TEXT, CATPPUCCIN_BASE, fb);
  print("Hello, World!", mbi->framebuffer_width - 10 - 13 * CHAR_SIZE, mbi->framebuffer_height - 10 - CHAR_SIZE_2 - 10 - CHAR_SIZE, CATPPUCCIN_TEXT, CATPPUCCIN_BASE, fb);

  Term term("", 5, 5, fb, {}, true);
  term.putChar('A', CATPPUCCIN_TEXT, CATPPUCCIN_BASE);
  term.putChar('b', CATPPUCCIN_TEXT, CATPPUCCIN_BASE);
  term.putChar('C', CATPPUCCIN_TEXT, CATPPUCCIN_BASE);

  memcpy((uint32_t*)mbi->framebuffer_addr, fb_addr, sizeof(fb_addr));

  inportb(0x60);

  while ((inportb(0x3f8 + 5) & 0x20) == 0) {}
  outportb(0x3f8, 'H');
  while ((inportb(0x3f8 + 5) & 0x20) == 0) {}
  outportb(0x3f8, 'i');

  // asm("int $33");

  for (;;) {
    asm("hlt");
  }
}
}
