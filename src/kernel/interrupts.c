#include "../../include/kernel/interrupts.h"
#include "../../include/kernel/buffers.h"
#include "../../include/drivers/keyboard.h"

int timer_ticks = 0;

void int_08() {
	timer_ticks++;
	return;
}

void int_09(int scanCode) {
	char c = scanCodeToAscii(scanCode);
	if(c != NOP) {
		add_to_keyboard_buffer(c);
	}
	return;
}