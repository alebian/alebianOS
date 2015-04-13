#ifndef _mouse_
#define _mouse_

#include "../kernel/defs.h"

typedef struct{
	int actual_x;
	int actual_y;
	int last_x;
	int last_y;
	int right_click;
	int left_click;
	int middle_click;
} __attribute__ ((aligned)) MousePosition;

void start_mouse(void);
int isMouseConnected(void);
void mouse_wait(byte);
byte mouse_read(void);
void mouse_write(byte);
void mouse_handler(byte);
void disableMousePackets(void);
void enableMousePackets(void);
MousePosition getMousePosition(void);

#endif