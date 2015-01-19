#ifndef _ints_
#define _ints_

void int_00(void);
void int_01(void);
void int_03(void);
void int_04(void);
void int_05(void);
void int_06(void);
void int_07(void);
void int_08(void);
void int_09(void);
void int_10(void);
void int_11(void);
void int_12(void);
void int_13(void);
void int_14(int, int);
void int_16(void);
void irq_00(void);
void irq_01(int);
void irq_12(int);
long getTicks(void);
void k_enableMouse(void);
void k_disableMouse(void);
int k_isMouseEnabled(void);

#endif