/*
* Copyright 2015 Alejandro Bezdjian
* Based in JS-OS Copyright 2013 JS-OS <js@duck-squirell>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _mouse_
#define _mouse_

#include "../../../common.h"

typedef struct{
	int actual_x;
	int actual_y;
	int last_x;
	int last_y;
	int right_click;
	int left_click;
	int middle_click;
} __attribute__ ((aligned)) MousePosition;

int init_mouse(void);
int checkConnection(void);
int isMouseConnected(void);
void mouse_wait(uint8_t);
uint8_t mouse_read(void);
void mouse_write(uint8_t);
void mouse_handler(uint8_t);
void disableMousePackets(void);
void enableMousePackets(void);
MousePosition getMousePosition(void);

#endif