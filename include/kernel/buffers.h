#ifndef _buffers_
#define _buffers_

#include "defs.h"

typedef unsigned char ascii;

typedef struct{
	char vec[KEYBOARD_BUFFER_SIZE]; 
	int read;
	int write;
}keyboard_buffer;

keyboard_buffer k_buffer;

void initializeKeyboardBuffer();
unsigned char get_char_from_keyboard_buffer();
void add_to_keyboard_buffer(unsigned char c);

#endif