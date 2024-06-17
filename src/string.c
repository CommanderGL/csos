#include "string.h"

void memcpy(void* dest, void* src, size_t n) {
  char* csrc = (char*)src;
  char* cdest = (char*)dest;

  for (int i = 0; i < (int)n; i++)
    cdest[i] = csrc[i];
}

size_t strlen(const char* str) {
  size_t len = 0;
  while (str[len])
    len++;
  return len;
}
