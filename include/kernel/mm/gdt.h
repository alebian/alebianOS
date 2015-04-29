#ifndef _gdt_
#define _gdt_

#include "../kernel.h"

typedef struct {
   u16int limit_low;           // The lower 16 bits of the limit.
   u16int base_low;            // The lower 16 bits of the base.
   u8int  base_middle;         // The next 8 bits of the base.
   u8int  access;              // Access flags, determine what ring this segment can be used in.
   u8int  granularity;
   u8int  base_high;           // The last 8 bits of the base.
} __attribute__((packed)) GDT_ENTRY;

typedef struct {
   u16int limit;               // The upper 16 bits of all selector limits.
   u32int base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed)) GDT_PTR;

void setup_GDT(void);
void gdt_set_gate(s32int, u32int, u32int, u8int, u8int);

#endif