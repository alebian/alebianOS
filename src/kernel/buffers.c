#include "../../include/kernel/buffers.h"
#include "../../include/kernel/k_libasm.h"
#include "../../include/kernel/defs.h"

void initializeKeyboardBuffer(){
	k_buffer.read=0;
	k_buffer.write=0;
}

void add_to_keyboard_buffer(unsigned char c) {
	if(k_buffer.write==KEYBOARD_BUFFER_SIZE){
		k_buffer.write=0;
	}
	k_buffer.vec[k_buffer.write]=c;
	k_buffer.write++;
	return;
}

unsigned char get_char_from_keyboard_buffer() {
	int c;
	if(k_buffer.read>KEYBOARD_BUFFER_SIZE){
		k_buffer.read=0;
	}
	if(k_buffer.read==k_buffer.write){
		initializeKeyboardBuffer();
		return 0;
	}
	c = k_buffer.vec[k_buffer.read];
	k_buffer.read++;
	return c;
}