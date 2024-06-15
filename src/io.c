#include "io.h"

__inline__ void outportb(dword port, byte value) {
  __asm__ __volatile__("outb %%al,%%dx" ::"d"(port), "a"(value));
}

__inline__ void outportw(dword port, dword value) {
  __asm__ __volatile__("outw %%ax,%%dx" ::"d"(port), "a"(value));
}

__inline__ byte inportb(dword port) {
  byte value;
  __asm__ __volatile__("inb %w1,%b0" : "=a"(value) : "d"(port));
  return value;
}
