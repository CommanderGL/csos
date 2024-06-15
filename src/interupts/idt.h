#include "../types.h"

#ifndef IDT_H
#define IDT_H

void loadIDTR(void);
void addInt(int num, void (*handler)(), dword dpl);

#endif
