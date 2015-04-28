#ifndef _buffers_
#define _buffers_

#include "kernel.h"

typedef struct{
	unsigned char vec[KEYBOARD_BUFFER_SIZE]; 
	int read;
	int write;
} __attribute__ ((aligned)) keyboard_buffer;

void start_keyboard_buffer(void);
void restart_keyboard_buffer(void);
unsigned char get_char_from_keyboard_buffer(void);
void add_to_keyboard_buffer(unsigned char);

#endif