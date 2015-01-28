#ifndef _idt_
#define _idt_

#include "defs.h"

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

void setup_IDT(void);
void setup_IDT_content(void);
void remapIRQ(void);
void setup_IDT_entry(INT_DESCR*, byte, dword, byte, byte);
void setup_IDTR(void);
void setup_PIC(void);

#endif