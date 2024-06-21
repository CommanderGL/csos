#include <stdbool.h>
#include <stddef.h>

#ifndef MM_H
#define MM_H

typedef struct {
  bool free;
  unsigned int size;
} allocation_t;

void mm_init(void);
void* malloc(size_t size);

#endif
