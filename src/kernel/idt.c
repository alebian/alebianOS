#include "../../include/system.h"

static INT_DESCR idt[IDT_SIZE];
static IDTR idtr;

/*
 * In this file the IDT is created and filled
 * The handlers loaded in the IDT are in interrupts.c
 * The PIC masks are handled here and the IRQs remaped 
 */
void setup_IDT(){
	/* Loads IDTR */
	setup_IDTR();
	/* Loads IDT */
	setup_IDT_content();
	/* Set PIC masks */
	setup_PIC();
	return;
}

void setup_IDTR(){
	idtr.base = 0;
	idtr.base += (dword) &idt;
	idtr.limit = sizeof(idt) - 1;
	_lidt(&idtr);
	// start IDT with 0
	memset(&idt, 0, sizeof(INT_DESCR)*IDT_SIZE);
	return;
}

/*
 * Loads the IDT with all the handlers
 */
void setup_IDT_content(){
	remapIRQ();
	//	Divide error
	setup_IDT_entry(0, 0x08, (dword) &int_00, ACS_EXC);
	//	Debug Exceptions
	setup_IDT_entry(1, 0x08, (dword) &int_01, ACS_EXC);
	//	Non maskable interrupt
	setup_IDT_entry(2, 0x08, (dword) &int_02, ACS_EXC);
	//	Breakpoint
	setup_IDT_entry(3, 0x08, (dword) &int_03, ACS_EXC);
	//	Overflow
	setup_IDT_entry(4, 0x08, (dword) &int_04, ACS_EXC);
	//	Bounds Check
	setup_IDT_entry(5, 0x08, (dword) &int_05, ACS_EXC);
	//	Invalid Opcode
	setup_IDT_entry(6, 0x08, (dword) &int_06, ACS_EXC);
	//	Coprocessor Not Available
	setup_IDT_entry(7, 0x08, (dword) &int_07, ACS_EXC);
	//	Double Fault
	setup_IDT_entry(8, 0x08, (dword) &int_08, ACS_EXC);
	//	Coprocessor Segment Overrun
	setup_IDT_entry(9, 0x08, (dword) &int_09, ACS_EXC);
	//	Invalid TSS
	setup_IDT_entry(10, 0x08, (dword) &int_10, ACS_EXC);
	//	Segment Not Present
	setup_IDT_entry(11, 0x08, (dword) &int_11, ACS_EXC);
	//	Stack Exception
	setup_IDT_entry(12, 0x08, (dword) &int_12, ACS_EXC);
	//	General Protection Exception(Triple Fault)
	setup_IDT_entry(13, 0x08, (dword) &int_13, ACS_EXC);
	//	Page Fault
	setup_IDT_entry(14, 0x08, (dword) &_int_14_hand, ACS_EXC);
	//	Coprocessor Error
	setup_IDT_entry(16, 0x08, (dword) &int_16, ACS_EXC);
	//	Syscalls
	setup_IDT_entry(128, 0x08, (dword) &_int_80_hand, ACS_EXC);
	//	IRQ0: timer tick
	setup_IDT_entry(32, 0x08, (dword) &_irq_00_hand, ACS_INT);
	//	IRQ1: keyboard
	setup_IDT_entry(33, 0x08, (dword) &_irq_01_hand, ACS_INT);
	//	IRQ12: mouse
	setup_IDT_entry(44, 0x08, (dword) &_irq_12_hand, ACS_INT);
	return;
}

/*
 * Remaps the IRQ table
 * From 32 to 47
 */
void remapIRQ(){
	_outb(0x20, 0x11);
	_outb(0xA0, 0x11);
	_outb(0x21, 0x20);
	_outb(0xA1, 0x28);
	_outb(0x21, 0x04);
	_outb(0xA1, 0x02);
	_outb(0x21, 0x01);
	_outb(0xA1, 0x01);
	_outb(0x21, 0x0);
	_outb(0xA1, 0x0);
}

/*
 * Sets an IDT Descriptor
 *
 * Arguments: 
 *	IDT's position
 *	Descriptor's selector
 *	Handler pointer	
 *	Access level
 */
void setup_IDT_entry (u8int num, byte selector, dword offset, byte access){
	idt[num].selector = selector;
  	idt[num].offset_l = offset & 0xFFFF;
  	idt[num].offset_h = offset >> 16;
  	idt[num].access = access;
  	idt[num].zero = 0;
}

/*
 * Sets the PIC masks
 * PIC1: Timer tick, Keyboard and second PIC
 * PIC2: CMOS and Mouse
 */
void setup_PIC(){
	_Cli();
	_maskPIC1(0xF8);
    _maskPIC2(0xEE);
	_Sti();
	return;
}