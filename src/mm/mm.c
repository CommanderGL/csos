#include "mm.h"
#include "../graphics/catppuccin.h"
#include "../graphics/graphics.h"
#include "../interupts/exceptions.h"
#include "stdlib.h"

extern struct Framebuffer direct_fb;

extern allocation_t endkernel;

#define PAGE_SIZE 4096
#define MAX_ALLOCATION PAGE_SIZE * 0xfff

void mm_init() {
  endkernel = (allocation_t){true, MAX_ALLOCATION};
}

void* malloc(size_t size) {
  allocation_t* endptr = &endkernel;
  while (endptr - &endkernel < MAX_ALLOCATION) {
    if (endptr->free == true && endptr->size >= size) {
      if (endptr->size == size) {
        endptr->free = false;
        return endptr + sizeof(allocation_t);
      }
      (endptr + size)->free = true;
      (endptr + size)->size = endptr->size - size;
      endptr->size = size;
      endptr->free = false;
      return endptr + sizeof(allocation_t);
    }
    endptr += endptr->size;
  }

  panic("Ran Out of Memory");
  return 0;
}
