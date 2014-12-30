#include "../../include/kernel/kernel.h"
#include "../../include/kernel/k_libasm.h"
#include "../../include/kernel/k_libc.h"
#include "../../include/kernel/interrupts.h"
#include "../../include/kernel/defs.h"
#include "../../include/kernel/buffers.h"
#include "../../include/drivers/video.h"
#include "../../include/shell.h"
#include "../../include/lib/stdio.h"

DESCR_INT idt[0xA];
IDTR idtr;

/**********************************************
kmain() 
C code entry point.
*************************************************/

int kmain() {
	clearScreen();
	showLoadingScreen();

	setup_paging();

	/* Loads IDT */
	setup_IDT_content();
	
	/* Loads IDTR */
	setup_IDTR();

	/* Set PIC masks */
	setup_PIC();

	setup_keyboard_buffer();
	
	clearScreen();
	shell();
	
	return 0;
}

void setup_paging(){
	printf("%s", "Starting paging... ");
	unsigned long *page_directory = (unsigned long *) PAGE_DIRECTORY_ADDRESS;
	unsigned long *page_table = (unsigned long *) PAGE_TABLE_ADDRESS;
	unsigned long address=0;
	int i;
	/* First 4MB of memory */
	for(i=0; i<1024; i++)
	{
		page_table[i] = address | 3; // Attribute set to: supervisor level, read/write, present (011 in binary)
		address = address + PAGE_SIZE;
	};
	/* Fill the first entry of the page directory */
	page_directory[0] = PAGE_TABLE_ADDRESS;
	page_directory[0] = page_directory[0] | 3;
	/* Fill the rest of the page directory */
	for(i=1; i<1024; i++)
	{
		page_directory[i] = 0 | 2; // Attribute set to: supervisor level, read/write, not present (010 in binary)
	};
	_write_cr3(PAGE_DIRECTORY_ADDRESS); 
	_write_cr0(_read_cr0() | 0x80000000); // Set the paging bit in CR0 to 1
	printf("%s\n\n", "DONE");
};

void setup_keyboard_buffer() {
	printf("%s", "Starting keyboard... ");
	initializeKeyboardBuffer();
	int j;
	for (j = 0; j < KEYBOARD_BUFFER_SIZE; j++) {
		k_buffer.vec[j] = 0;
	}
	printf("%s\n\n", "DONE");
	return;
}

void setup_IDT_content() {
	printf("%s", "Loading IDT... ");
	//	IRQ0: timer tick
	setup_IDT_entry(&idt[0x08], 0x08, (dword) &_int_08_hand, ACS_INT, 0);
	//	IRQ1: keyboard
	setup_IDT_entry(&idt[0x09], 0x08, (dword) &_int_09_hand, ACS_INT, 0);
	printf("%s\n\n", "DONE");
	return;
}

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access, byte zero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->zero = zero;
}

void setup_IDTR() {
	printf("%s", "Loading IDTR... ");
	idtr.base = 0;
	idtr.base += (dword) &idt;
	idtr.limit = sizeof(idt) - 1;
	_lidt(&idtr);
	printf("%s\n\n", "DONE");
	return;
}

void setup_PIC(){
	printf("%s", "Setting PIC... ");
	_Cli();
	_maskPIC1(0xFC);
    _maskPIC2(0xFF);
	_Sti();
	printf("%s\n\n", "DONE");
	return;
}