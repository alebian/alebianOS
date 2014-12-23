#include "../../include/kernel/kernel.h"
#include "../../include/kernel/kasm.h"
#include "../../include/kernel/kc.h"
#include "../../include/kernel/interrupts.h"
#include "../../include/kernel/defs.h"
#include "../../include/drivers/video.h"
#include "../../include/shell.h"
#include "../../include/lib/stdio.h"

DESCR_INT idt[0xA];
IDTR idtr;

void setup_IDT_content() {
	//	IRQ0: timer tick
	setup_IDT_entry(&idt[0x08], 0x08, (dword) &_int_08_hand, ACS_INT, ACS_USED);
	//	IRQ1: keyboard
	setup_IDT_entry(&idt[0x09], 0x08, (dword) &_int_09_hand, ACS_INT, ACS_USED);
}

void setup_IDTR() {
	idtr.base = 0;
	idtr.base += (dword) &idt;
	idtr.limit = sizeof(idt) - 1;
	_lidt(&idtr);
}

/**********************************************
kmain() 
C code entry point.
*************************************************/

int kmain() {
	clearScreen();
	showLoadingScreen();

	/* Loads IDT */
	printf("%s", "Loading IDT... ");
	setup_IDT_content();
	printf("%s\n\n", "DONE");

	/* Loads IDTR */
	printf("%s", "Loading IDTR... ");
	setup_IDTR();
	printf("%s\n\n", "DONE");

	/* Set PIC masks */
	printf("%s", "Setting PIC... ");
	_Cli();
	_maskPIC1(0xFC);
    _maskPIC2(0xFF);
	_Sti();
	printf("%s\n\n", "DONE");
	
	//while(wait<20){}
	clearScreen();
	shell();
	
	return 0;
}