#include "timer.h"
#include "../interupts/idt.h"
#include "../interupts/ints.h"
#include "../io.h"
#include <stdint.h>

interval_handler interval_handlers[0xff];
uint8_t interval_handlers_length = 0;

void timer_phase(double hz) {
  int divisor = 1193182 / hz;
  outportb(0x43, 0x36);
  outportb(0x40, divisor & 0xFF);
  outportb(0x40, divisor >> 8);
}

int timer_ticks = 0;

void timer_handler_c() {
  timer_ticks++;

  for (int i = 0; i < interval_handlers_length; i++) {
    if (timer_ticks % interval_handlers[i].interval == 0) {
      interval_handlers[i].handler();
    }
  }

  outportb(0x20, 0x20);
}

void timer_install() {
  timer_phase(1000);
  addInt(32, timer_handler, 0);
}

int add_interval_handler(double interval, void (*handler)()) {
  if (interval_handlers_length == 0xff) return 1;
  interval_handlers[interval_handlers_length].interval = interval;
  interval_handlers[interval_handlers_length].handler = handler;
  interval_handlers_length++;
  return 0;
}
