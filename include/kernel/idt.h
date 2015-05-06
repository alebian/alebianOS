#ifndef _idt_
#define _idt_

#include "kernel.h"

#define IDT_SIZE 256

/* Interrupt descriptor */
typedef struct {
  word      offset_l;
  word      selector;
  byte      zero;
  byte      access;
  word      offset_h;
} __attribute__ ((packed)) INT_DESCR;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} __attribute__ ((packed)) IDTR;

int init_IDT(void);
void init_IDT_content(void);
void remapIRQ(void);
void init_IDT_entry(u8int, byte, dword, byte);
void init_IDTR(void);
void setup_PIC(void);

#endif