#include "serial.h"
#include "io.h"
#include "string.h"

void serial_putChar(unsigned char ch) {
  while ((inportb(0x3f8 + 5) & 0x20) == 0) {}
  outportb(0x3f8, ch);
}

void serial_writeString(const char* data) {
  for (size_t i = 0; i < strlen(data); i++) {
    serial_putChar(data[i]);
  }
}

void serial_printLn(const char* data) {
  serial_writeString(data);
  serial_putChar('\n');
}
