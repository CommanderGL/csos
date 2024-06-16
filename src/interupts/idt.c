#include "idt.h"
#include "descriptor.h"

IDTR idtr;
x86_interrupt idt[256];

void loadIDTR() {
  idtr.limit = 256 * (sizeof(x86_interrupt) - 1);
  idtr.base = idt;

  asm volatile("lidt %0 " ::"m"(idtr));
}

void addInt(int num, void (*handler)(), dword dpl) {
  word selector = 0x08;
  word settings = 0;
  dword offset = (dword)(long)handler;

  asm volatile("movw %%cs,%0" : "=g"(selector));

  switch (dpl) {
  case 0:
    settings = INT_0;
    break;
  case 1:
  case 2:
  case 3:
    settings = INT_3;
    break;
  }

  /* set actual values of int */
  idt[num].low_offset = (offset & 0xFFFF);
  idt[num].selector = selector;
  idt[num].settings = settings;
  idt[num].high_offset = (offset >> 16);
}
