/*
* Copyright 2014 Alejandro Bezdjian
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

#ifndef _idt_
#define _idt_

#include "system.h"
#include "../../common/common.h"

#define IDT_SIZE 256
#define PIC1_CMD                    0x20
#define PIC1_DATA                   0x21
#define PIC2_CMD                    0xA0
#define PIC2_DATA                   0xA1
#define PIC_READ_IRR                0x0a    /* OCW3 irq ready next CMD read */
#define PIC_READ_ISR                0x0b    /* OCW3 irq service next CMD read */

#define IRQ_PIT 					0
#define IRQ_KEYBOARD 				1
#define IRQ_CASCADE 				2
#define IRQ_COM2	 				3
#define IRQ_COM1	 				4
#define IRQ_LPT2	 				5
#define IRQ_FLOPPY	 				6
#define IRQ_LPT1	 				7
#define IRQ_CMOS	 				8
#define IRQ_MOUSE	 				12
#define IRQ_FPU		 				13
#define IRQ_MASTER_DISK				14
#define IRQ_SLAVE_DISK 				15

/* Interrupt descriptor */
typedef struct {
  sint16_t      offset_l;
  sint16_t      selector;
  uint8_t      zero;
  uint8_t      access;
  sint16_t      offset_h;
} __attribute__ ((packed)) INT_DESCR;

/* IDTR  */
typedef struct {
  sint16_t  limit;
  sint32_t base;
} __attribute__ ((packed)) IDTR;

int init_IDT(void);
void init_IDT_content(void);
void remapIRQ(void);
void init_IDT_entry(uint8_t, uint8_t, sint32_t, uint8_t);
void init_IDTR(void);
void restart_PIC(void);
void IRQ_set_mask(unsigned char);
void IRQ_clear_mask(unsigned char);

#endif