#include "pic.h"
#include "../io.h"

/* void remapPIC(int pic1, int pic2) {
  byte md, sd;

  md = inportb(MASTERDATA);
  sd = inportb(SLAVEDATA);

  outportb(MASTER, EOI);

  outportb(MASTER, ICW1_INIT + ICW1_ICW4);
  outportb(SLAVE, ICW1_INIT + ICW1_ICW4);

  outportb(MASTERDATA, pic1);
  outportb(SLAVEDATA, pic2);

  outportb(MASTERDATA, 0x04);
  outportb(SLAVEDATA, 0x02);

  outportb(MASTERDATA, ICW4_8086);
  outportb(SLAVEDATA, ICW4_8086);

  outportb(MASTERDATA, md);
  outportb(SLAVEDATA, sd);
} */

void remapPIC(int pic1, int pic2) {
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

void maskIRQ(byte irq) {
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
}

void unmaskIRQ(byte irq) {
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
}
