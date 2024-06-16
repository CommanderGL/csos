#include "pic.h"
#include "../io.h"

void remapPIC() {
  outportb(0x20, 0x11); /* ICW1 */
  outportb(0xA0, 0x11);
  outportb(0x21, 0x20); /* remap IRQ0~7 to 32~39 in interrupt vector */
  outportb(0xA1, 0x28); /* remap IRQ8~15 to 40~47 in interrupt vector */

  outportb(0x21, 0x04); /* ICW3 */
  outportb(0xA1, 0x02);

  outportb(0x21, 0x01); /* ICW4 */
  outportb(0xA1, 0x01);

  outportb(0x21, 0x01); /* enable all interrupt */
  outportb(0xA1, 0x00);
}

/* void maskIRQ(byte irq) {
  if (irq == ALL) {
    outportb(MASTERDATA, 0xFF);
    outportb(SLAVEDATA, 0xFF);
    return;
  }
  irq = irq | (1 << irq);
  if (irq < 8) {
    outportb(MASTERDATA, irq & 0xFF);
    return;
  }
  outportb(SLAVEDATA, irq >> 8);
} */

void maskIRQ(uint8_t irq) {
  uint16_t port;
  uint8_t value;

  if (irq < 8) {
    port = 0x21;
  } else {
    port = 0xa1;
    irq -= 8;
  }
  value = inportb(port) | (1 << irq);
  outportb(port, value);
}

/* void unmaskIRQ(byte irq) {
  if (irq == ALL) {
    outportb(MASTERDATA, 0x00);
    outportb(SLAVEDATA, 0x00);
    return;
  }
  irq = irq & (1 << irq);
  if (irq < 8) {
    outportb(MASTERDATA, irq & 0xFF);
    return;
  }
  outportb(SLAVEDATA, irq >> 8);
} */

void unmaskIRQ(uint8_t irq) {
  uint16_t port;
  uint8_t value;

  if (irq < 8) {
    port = 0x21;
  } else {
    port = 0xa1;
    irq -= 8;
  }
  value = inportb(port) & ~(1 << irq);
  outportb(port, value);
}
