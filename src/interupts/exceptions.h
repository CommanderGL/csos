#include <stdbool.h>

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

void loadExceptions();
void panic(const char* message);

void int_00(void);
void int_01(void);
void int_02(void);
void int_03(void);
void int_04(void);
void int_05(void);
void int_06(void);
void int_07(void);
void int_08(void);
void int_10(void);
void int_11(void);
void int_12(void);
void int_13(void);
void int_14(void);
void int_16(void);
void int_17(void);
void int_18(void);
void int_19(void);

#endif
