/*
* Copyright 2015 Alejandro Bezdjian
* Based on code from Bran's kernel development tutorials.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _gdt_
#define _gdt_

#include "system.h"
#include "../../common/common.h"

#define GDT_SIZE 5

typedef struct {
   uint16_t limit_low;           // The lower 16 bits of the limit.
   uint16_t base_low;            // The lower 16 bits of the base.
   uint8_t  base_middle;         // The next 8 bits of the base.
   uint8_t  access;              // Access flags, determine what ring this segment can be used in.
   uint8_t  granularity;
   uint8_t  base_high;           // The last 8 bits of the base.
} __attribute__((packed)) GDT_ENTRY;

typedef struct {
   uint16_t limit;               // The upper 16 bits of all selector limits.
   uint32_t base;                // The address of the first gdt_entry_t struct.
} __attribute__((packed)) GDT_PTR;

/* Segment descriptor */
typedef struct {
	sint16_t limit;
	sint16_t base_l;
	uint8_t base_m;
	uint8_t access;
	uint8_t attribs;
	uint8_t base_h;
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
void gdt_set_gate(sint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

#endif