//         Based on code from Bran's kernel development tutorials.

#ifndef _gdt_
#define _gdt_

#include "kernel.h"

#define GDT_SIZE 5

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

/* Segment descriptor */
typedef struct {
	word limit;
	word base_l;
	byte base_m;
	byte access;
	byte attribs;
	byte base_h;
} __attribute__ ((packed)) DESCR_SEG;

/* Segment access rights flags */
#define ACS_PRESENT     0x80            /* segment present */
#define ACS_CSEG        0x18            /* code segment */
#define ACS_DSEG        0x10            /* data segment */
#define ACS_READ        0x02            /* read segment */
#define ACS_WRITE       0x02            /* write segment */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386     0x0E        /* Interrupt GATE 32 bits */
#define ACS_EXC_386     0x0F            /* Exception GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )
#define ACS_EXC         ( ACS_PRESENT | ACS_EXC_386 )
#define ACS_USED        0x10            /* Interrupt programed */
#define ACS_CODE        ( ACS_PRESENT | ACS_CSEG | ACS_READ )
#define ACS_DATA        ( ACS_PRESENT | ACS_DSEG | ACS_WRITE )
#define ACS_STACK       ( ACS_PRESENT | ACS_DSEG | ACS_WRITE )

int init_GDT(void);
void gdt_set_gate(s32int, u32int, u32int, u8int, u8int);

#endif