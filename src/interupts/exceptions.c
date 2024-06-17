#include "exceptions.h"
#include "../catppuccin.h"
#include "../graphics.h"
#include "../io.h"
#include "../keyboard/keyboard.h"
#ifdef __MULTIBOOT__
#include "../multiboot.h"
#endif
#include "../string.h"
#include "idt.h"
#include "ints.h"
#include "pic.h"

extern uint32_t fb_addr[SCREEN_WIDTH * SCREEN_HEIGHT + 1];
extern struct Framebuffer fb;
#ifdef __MULTIBOOT__
extern multiboot_info_t* mbi;
#endif

void loadExceptions() {
  addInt(0, int00, 0);
  addInt(1, int01, 0);
  addInt(2, int02, 0);
  addInt(3, int03, 0);
  addInt(4, int04, 0);
  addInt(5, int05, 0);
  addInt(6, int06, 0);
  addInt(7, int07, 0);
  addInt(8, int08, 0);
  addInt(9, int09, 0);
  addInt(10, int10, 0);
  addInt(11, int11, 0);
  addInt(12, int12, 0);
  addInt(13, int13, 0);
  addInt(14, int14, 0);
  addInt(16, int16, 0);
  addInt(17, int17, 0);
  addInt(18, int18, 0);
  addInt(19, int19, 0);
  addInt(20, 0, 0);
  addInt(21, 0, 0);
  addInt(22, 0, 0);
  addInt(23, 0, 0);
  addInt(24, 0, 0);
  addInt(25, 0, 0);
  addInt(26, 0, 0);
  addInt(27, 0, 0);
  addInt(28, 0, 0);
  addInt(29, 0, 0);
  addInt(30, 0, 0);
  addInt(31, 0, 0);
}

void panic(const char* message) {
  clr_screen(CATPPUCCIN_BASE, fb);

  const int lines = 11;
  const int yOffset = fb.height / 2 - lines * CHAR_SIZE_2 / 2;

  print2("<SYSTEM ERROR>", fb.width / 2 - strlen("<SYSTEM ERROR>") * CHAR_SIZE_2 / 2, yOffset, CATPPUCCIN_RED, CATPPUCCIN_BASE, fb);
  print2("An exception Has Occured (your system has messed up):", fb.width / 2 - strlen("An exception Has Occured (your system has messed up):") * CHAR_SIZE_2 / 2, yOffset + CHAR_SIZE_2 * 2, CATPPUCCIN_RED, CATPPUCCIN_BASE, fb);
  print2(message, fb.width / 2 - strlen(message) * CHAR_SIZE_2 / 2, yOffset + CHAR_SIZE_2 * 4, CATPPUCCIN_RED, CATPPUCCIN_BASE, fb);
  print2("<SYSTEM HALTED>", fb.width / 2 - strlen("<SYSTEM HELATED>") * CHAR_SIZE_2 / 2, yOffset + CHAR_SIZE_2 * 10, CATPPUCCIN_RED, CATPPUCCIN_BASE, fb);

#ifdef __MULTIBOOT__
  memcpy((uint32_t*)mbi->framebuffer_addr, fb_addr, sizeof(fb_addr));
#endif

  asm("cli\n");
  asm("hlt\n");
}

void int_00(void) {
  panic("Divide By Zero Error");
}

void int_01(void) {
  panic("Debug Error");
}

void int_02(void) {
  panic("NMI Interrupt");
}

void int_03(void) {
  panic("Breakpoint");
}

void int_04(void) {
  panic("Overflow");
}

void int_05(void) {
  panic("BOUND Range Exceeded");
}

void int_06(void) {
  panic("Invalid Opcode");
}

void int_07(void) {
  panic("Device Not Available");
}

void int_08(void) {
  panic("Double Fault");
}

void int_09(void) {
  panic("Coprocessor Segment Overrun");
}

void int_10(void) {
  panic("Invalid TSS");
}

void int_11(void) {
  panic("Segment Not Present");
}

void int_12(void) {
  panic("Stack Segment Fault");
}

void int_13(void) {
  panic("General Protection Fault");
}

void int_14(void) {
  panic("Page Fault");
}

void int_16(void) {
  panic("FPU Floating-Point Error");
}

void int_17(void) {
  panic("Alignment Check");
}

void int_18(void) {
  panic("Machine Check");
}

void int_19(void) {
  panic("SIMD Floating-Point");
}
