#include "../../include/system.h"

static long timer_ticks = 0;
static int mouse_enabled = 0;

void int_00(){
	k_panic("Divide error exception.");
	/*
	int c;
	k_printerror("%s\n", "Divide error exception, please press enter to continue.\n");
	while((c=getchar()) != '\n'){}
	//shell();*/
	return;
}

void int_01(){
	k_panic("Debug exception.");
	return;
}

void int_02(){
	//k_panic("Non maskable interrupt.");
	k_exitScreen();
	return;
}

void int_03(){
	k_panic("Breakpoint exception.");
	return;
}

void int_04(){
	k_panic("Overflow exception.");
	return;
}

void int_05(){
	k_panic("Bounds check exception.");
	return;
}

void int_06(){
	k_panic("Invalid opcode exception.");
	return;
}

void int_07(){
	k_panic("Coprocessor not available exception.");
	return;
}

void int_08(){
	k_panic("Double fault exception.");
	return;
}

void int_09(){
	k_panic("Coprocessor segment overrun exception.");
	return;
}

void int_10(){
	k_panic("Invalid TSS exception.");
	return;
}

void int_11(){
	k_panic("Segment not present exception.");
	return;
}

void int_12(){
	k_panic("Stack exception.");
	return;
}

void int_13(){
	k_panic("General protection exception.");
	return;
}

void int_14(int errorcode, int cr2){
	int c;
	k_printerror("Page fault exception.\n");
	if(errorcode & 0x01){ // Present bit is set
		if(errorcode & 0x02){ // R/W bit is set
			if(errorcode & 0x04){ // U/S bit is set
				// 1 1 1
				k_printerror("User process tried to write a page and caused a protection fault.\n");
			}else{ // U/S bit is not set
				// 0 1 1
				k_printerror("Supervisory process tried to write a page and caused a protection fault.\n");
			}
		}else{// R/W bit is not set
			if(errorcode & 0x04){ // U/S bit is set
				// 1 0 1
				k_printerror("User process tried to read a page and caused a protection fault.\n");
			}else{ // U/S bit is not set
				// 0 0 1
				k_printerror("Supervisory process tried to read a page and caused a protection fault.\n");
			}
		}
	}else{ // Present flag is not set
		if(errorcode & 0x02){ // R/W bit is set
			if(errorcode & 0x04){ // U/S bit is set
				// 1 1 0
				k_printerror("User process tried to write to a non-present page entry.\n");
			}else{ // U/S bit is not set
				// 0 1 0
				k_printerror("Supervisory process tried to write to a non-present page entry.\n");
			}
		}else{ // R/W bit is not set
			if(errorcode & 0x04){ // U/S bit is set
				// 1 0 0
				k_printerror("User process tried to read a non-present page entry.\n");
			}else{ // U/S bit is not set
				// 0 0 0
				k_printerror("Supervisory process tried to read a non-present page entry.\n");
			}
		}
	}
	while((c=getchar()) != '\n'){}
	return;
}

void int_16(){
	k_panic("Coprocessor error exception.");
	return;
}

void int_80(int eax, int ebx, int ecx, int edx){
	k_syscall(eax, ebx, ecx, edx);
	return;
}

void irq_00(){
	timer_ticks++;
	if(k_isLoading()){
		k_LoadingScreenEffect();
	}
	if(k_isReadyToShell()){
		if(timer_ticks%20 == 0){
			shell_updateStartBar();
		}
	}
	return;
}

void irq_01(int scanCode){
	unsigned char c = scanCodeToAscii(scanCode);
	if(c != NOP){
		k_syscall(4, 2, (dword)&c, 1);
	}
	_Sti();
	k_KeyboardListener();
	return;
}

void irq_12(int pkg){
	if(mouse_enabled){
		byte c = pkg;
		mouse_handler(c);
	}
	return;
}

long getTicks(){
	return timer_ticks;
}

void k_enableMouse(){
	if(isMouseConnected()){
		mouse_enabled = 1;
	}
	return;
}

void k_disableMouse(){
	mouse_enabled = 0;
	return;
}

int k_isMouseEnabled(){
	return mouse_enabled;
}