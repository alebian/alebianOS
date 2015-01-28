#include "../../include/kernel/buffers.h"
#include "../../include/kernel/k_libasm.h"
#include "../../include/kernel/defs.h"

static keyboard_buffer k_buffer;

void start_keyboard_buffer(){
	int j;
	restart_keyboard_buffer();
	for (j = 0; j < KEYBOARD_BUFFER_SIZE; j++){
		k_buffer.vec[j] = 0;
	}
	return;
}

void restart_keyboard_buffer(){
	k_buffer.read=0;
	k_buffer.write=0;
	return;
}

void add_to_keyboard_buffer(unsigned char c){
	k_buffer.vec[k_buffer.write++]=c;
	if(k_buffer.write>=KEYBOARD_BUFFER_SIZE){
		k_buffer.write=0;
	}
	return;
}

unsigned char get_char_from_keyboard_buffer(){
	unsigned char c;
	if(k_buffer.read==k_buffer.write){
		//restart_keyboard_buffer();
		return 0;
	}
	c = k_buffer.vec[k_buffer.read++];
	if(k_buffer.read>=KEYBOARD_BUFFER_SIZE){
		k_buffer.read=0;
	}
	return c;
}