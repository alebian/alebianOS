/*
* Copyright 2014, 2015 Alejandro Bezdjian
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

#include "include/system.h"

static long timer_ticks = 0;
static int mouse_enabled = 0;

void int_80h(int eax, int ebx, int ecx, int edx){
	k_syscall(eax, ebx, ecx, edx);
	return;
}

void irq_00(){
	timer_ticks++;
	if(timer_ticks%20 == 0){
		k_updateStartBar();
	}
	return;
}

void irq_01(int scanCode){
	unsigned char c = scanCodeToAscii(scanCode);
	if(k_sbarmenuopened()){
		k_closeStartMenu();
	}
	if(c != 0){
		k_syscall(4, 2, (sint32_t)&c, 1);
	}
	_Sti();
	k_KeyboardListener();
	return;
}

void irq_12(int pkg){
	if(k_isMouseEnabled()){
		uint8_t c = pkg;
		mouse_handler(c);
	}
	return;
}

long getTicks(){
	return timer_ticks;
}

int k_enableMouse(){
	if(isMouseConnected()){
		mouse_enabled = 1;
		return 1;
	}
	return 0;
}

int k_disableMouse(){
	mouse_enabled = 0;
	return 0;
}

int k_isMouseEnabled(){
	return mouse_enabled;
}