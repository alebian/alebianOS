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

#include "include/system.h"
#include "../common/common.h"

static INT_DESCR idt[IDT_SIZE];
static IDTR idtr;

/*
 * In this file the IDT is created and filled
 * The handlers loaded in the IDT are in interrupts.c
 * The PIC masks are handled here and the IRQs remaped 
 */
 
int init_IDT(){
	_Cli();
	/* Loads IDTR */
	init_IDTR();
	/* Loads IDT */
	init_IDT_content();
	/* Restart PIC masks */
	restart_PIC();
	/* Set required IRQ's */
	IRQ_clear_mask(IRQ_PIT);
	IRQ_clear_mask(IRQ_KEYBOARD);
	IRQ_clear_mask(IRQ_CASCADE);
	IRQ_clear_mask(IRQ_MOUSE);

	_Sti();
	return IDT_SIZE;
}

void init_IDTR(){
	idtr.base = 0;
	idtr.base += (sint32_t) &idt;
	idtr.limit = sizeof(idt) - 1;
	_lidt(&idtr);
	// start IDT with 0
	memset(&idt, 0, sizeof(INT_DESCR)*IDT_SIZE);
	return;
}

/* Loads the IDT with all the handlers */
void init_IDT_content(){
	remapIRQ();
	//	Divide error
	init_IDT_entry(0, 0x08, (sint32_t) &exc_00, ACS_EXC);
	//	Debug Exceptions
	init_IDT_entry(1, 0x08, (sint32_t) &exc_01, ACS_EXC);
	//	Non maskable interrupt
	init_IDT_entry(2, 0x08, (sint32_t) &exc_02, ACS_EXC);
	//	Breakpoint
	init_IDT_entry(3, 0x08, (sint32_t) &exc_03, ACS_EXC);
	//	Overflow
	init_IDT_entry(4, 0x08, (sint32_t) &exc_04, ACS_EXC);
	//	Bounds Check
	init_IDT_entry(5, 0x08, (sint32_t) &exc_05, ACS_EXC);
	//	Invalid Opcode
	init_IDT_entry(6, 0x08, (sint32_t) &exc_06, ACS_EXC);
	//	Coprocessor Not Available
	init_IDT_entry(7, 0x08, (sint32_t) &exc_07, ACS_EXC);
	//	Double Fault
	init_IDT_entry(8, 0x08, (sint32_t) &exc_08, ACS_EXC);
	//	Coprocessor Segment Overrun
	init_IDT_entry(9, 0x08, (sint32_t) &exc_09, ACS_EXC);
	//	Invalid TSS
	init_IDT_entry(10, 0x08, (sint32_t) &exc_10, ACS_EXC);
	//	Segment Not Present
	init_IDT_entry(11, 0x08, (sint32_t) &exc_11, ACS_EXC);
	//	Stack Exception
	init_IDT_entry(12, 0x08, (sint32_t) &exc_12, ACS_EXC);
	//	General Protection Exception(Triple Fault)
	init_IDT_entry(13, 0x08, (sint32_t) &exc_13, ACS_EXC);
	//	Page Fault
	init_IDT_entry(14, 0x08, (sint32_t) &_exc_14_hand, ACS_EXC);
	//	Coprocessor Error
	init_IDT_entry(16, 0x08, (sint32_t) &exc_16, ACS_EXC);
	//	Syscalls
	init_IDT_entry(0x80, 0x08, (sint32_t) &_int_80h_hand, ACS_EXC);
	//	IRQ0: timer tick
	init_IDT_entry(32, 0x08, (sint32_t) &_irq_00_hand, ACS_INT);
	//	IRQ1: keyboard
	init_IDT_entry(33, 0x08, (sint32_t) &_irq_01_hand, ACS_INT);
	//	IRQ12: mouse
	init_IDT_entry(44, 0x08, (sint32_t) &_irq_12_hand, ACS_INT);
	return;
}

/* Remaps the IRQ table */
void remapIRQ(){
	_outb(0x20, 0x11);
	_outb(0xA0, 0x11);
	// Set first PIC offset
	_outb(0x21, 0x20);
	// Set second PIC offset
	_outb(0xA1, 0x28);
	_outb(0x21, 0x04);
	_outb(0xA1, 0x02);
	_outb(0x21, 0x01);
	_outb(0xA1, 0x01);
	_outb(0x21, 0x0);
	_outb(0xA1, 0x0);
}

/* Sets an IDT Descriptor */
void init_IDT_entry (uint8_t num, uint8_t selector, sint32_t offset, uint8_t access){
	idt[num].selector = selector;
  	idt[num].offset_l = offset & 0xFFFF;
  	idt[num].offset_h = offset >> 16;
  	idt[num].access = access;
  	idt[num].zero = 0;
}

/* Restarts the PIC masks */
void restart_PIC(){
	_outb(0x21, 0xFF);
	_outb(0xA1, 0xFF);
	return;
}

/* Disables the desired IRQ */
void IRQ_set_mask(unsigned char IRQline){
    uint16_t port;
    uint8_t value;
 
    if(IRQline >= 0 && IRQline < 8) {
        port = PIC1_DATA;
    } 
    else if(IRQline >= 8 && IRQline < 16){
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = _inb(port) | (1 << IRQline);
    _outb(port, value);
    return;
}
 
/* Enables the desired IRQ */
void IRQ_clear_mask(unsigned char IRQline){
    uint16_t port;
    uint8_t value;
 
    if(IRQline >= 0 && IRQline < 8) {
        port = PIC1_DATA;
    } 
    else if(IRQline >= 8 && IRQline < 16){
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = _inb(port) & ~(1 << IRQline);
    _outb(port, value);
    return;
}