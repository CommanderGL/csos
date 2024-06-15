#include "../types.h"

#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

// Typee
#define INT_0 0x8E00
#define INT_3 0xEE00

typedef struct {
  word low_offset;
  word selector;
  word settings;
  word high_offset;
} __attribute__((packed)) x86_interrupt;

typedef struct {
  word limit;
  x86_interrupt* base;
} __attribute__((packed)) IDTR;

#endif
