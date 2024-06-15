#include "types.h"

#ifndef IO_H
#define IO_H

extern __inline__ void outportb(dword port, byte value);
extern __inline__ void outportw(dword port, dword value);
extern __inline__ byte inportb(dword port);

#endif
