#include "graphics/catppuccin.h"
#include "graphics/graphics.h"
#include "interupts/exceptions.h"
#include "interupts/idt.h"
#include "interupts/pic.h"
#include "io.h"
#include "keyboard/keyboard.h"
#include "mouse/mouse.h"
#include "multiboot2.h"
#include "stdlib.h"
#include "string.h"
#include "term.h"
#include "timer/timer.h"
#include <stdbool.h>
#include <stdint.h>

#if defined(__linux__)
#error \
  "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

extern void endkernel;

multiboot_uint8_t* mbi;
struct multiboot_tag_framebuffer* fb_info = NULL;

uint32_t fb_addr[SCREEN_WIDTH * SCREEN_HEIGHT + 1];
struct Framebuffer fb;
struct Framebuffer direct_fb;

void test_handler(Scancode sc, unsigned char ch) {
  if (sc & 0x80) return;

  term_putChar(ch, CATPPUCCIN_TEXT, ch == '\b' ? CATPPUCCIN_BASE : TRANSPARENT);
  memcpy((uint32_t*)fb_info->common.framebuffer_addr, fb_addr, sizeof(fb_addr));

  draw_cursor();
}

bool interval_toggle = false;
void test_interval() {
  interval_toggle = !interval_toggle;
  drawRectFill(SCREEN_WIDTH - 35, 180, 25, 50, interval_toggle ? CATPPUCCIN_SKY : CATPPUCCIN_BLUE, fb);
  memcpy((uint32_t*)fb_info->common.framebuffer_addr, fb_addr, sizeof(fb_addr));
  draw_cursor();
}

void kernel_main(unsigned long magic, multiboot_uint8_t* addr) {
  if (magic != MULTIBOOT2_BOOTLOADER_MAGIC) asm("hlt");
  mbi = addr;
  struct multiboot_tag* tag = (struct multiboot_tag*)(mbi + 8);
  if (tag == NULL) asm("hlt");
  while (tag->type != MULTIBOOT_TAG_TYPE_END) {
    if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
      fb_info = (struct multiboot_tag_framebuffer*)tag;
      break;
    }
    tag = (struct multiboot_tag*)((multiboot_uint8_t*)tag + ((tag->size + 7) & ~7));
  }
  if (fb_info == NULL) asm("hlt");

  fb.addr = fb_addr;
  fb.width = SCREEN_WIDTH;
  fb.height = SCREEN_HEIGHT;
  fb.pitch = SCREEN_WIDTH * 4;

  direct_fb.addr = (uint32_t*)fb_info->common.framebuffer_addr;
  direct_fb.width = fb_info->common.framebuffer_width;
  direct_fb.height = fb_info->common.framebuffer_height;
  direct_fb.pitch = fb_info->common.framebuffer_pitch;

  term_init("", 10, 10, SCREEN_WIDTH, SCREEN_HEIGHT, true, fb);

  asm("cli");
  remapPIC();
  maskIRQ(ALL);
  unmaskIRQ(KEYBOARD);
  unmaskIRQ(TIMER);
  unmaskIRQ(PS2MOUSE);
  loadExceptions();
  keyboard_init(kbm_interrupt);
  timer_install();
  mouse_init();
  loadIDTR();
  asm("sti");

  clr_screen(CATPPUCCIN_BASE, fb);

  drawLine(fb.width, 0, 0, fb.height, CATPPUCCIN_RED, fb);
  drawRectOutline(fb.width - 100 - 10, 10, 100, 50, CATPPUCCIN_SURFACE1, fb);
  drawRectFill(fb.width - 50 - 10, 10 + 50 + 10, 50, 100, CATPPUCCIN_GREEN, fb);
  print2("Hello, World!", fb.width - 10 - 13 * CHAR_SIZE_2, fb.height - 10 - CHAR_SIZE_2, CATPPUCCIN_TEXT, CATPPUCCIN_BASE, fb);
  print("Hello, World!", fb.width - 10 - 13 * CHAR_SIZE, fb.height - 10 - CHAR_SIZE_2 - CHAR_SIZE, CATPPUCCIN_TEXT, CATPPUCCIN_BASE, fb);
  // drawCircle(25, 10, SCREEN_HEIGHT - 60, CATPPUCCIN_PEACH, fb);

  memcpy((uint32_t*)fb_info->common.framebuffer_addr, fb_addr, sizeof(fb_addr));

  kb_handlers[kb_handlers_length] = test_handler;
  kb_handlers_length++;

  add_interval_handler(1000, test_interval);

  while (true) {}
}
