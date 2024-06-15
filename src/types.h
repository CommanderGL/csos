#include <stdint.h>

#ifndef TYPES_H
#define TYPES_H

#define byte unsigned char
#define word unsigned short
#define dword unsigned int

#define BIT_SET(var, bit) ((var) |= (1 << (bit)))
#define BIT_CLEAR(var, bit) ((var) &= ~(1 << (bit)))
#define IS_BIT_CLEAR(var, bit) (((var) & (1 << bit)) == 0)

typedef unsigned long ptr_t;

typedef struct bitmap {
  uint32_t* data;
  uint32_t size;
  uint32_t cache;
} bitmap_t;

#endif
