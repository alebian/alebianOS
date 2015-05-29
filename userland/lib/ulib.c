/*
* Copyright 2015 Alejandro Bezdjian
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

#include "../include/lib/ulib.h"
#include "../include/syscall.h"
#include "../../common.h"

void s_beep(){
	syscall(1100, 1, 0, 0); // Beep
	return;
}

void s_setKBlistener(int listener){
	syscall(1200, 1, listener, 0);
	return;
}

void s_printError(char* message){
	int i = 0;
	while(message[i]!=0){
		syscall(1000, 9, (uint32_t)&message[i], S_CHAR_COLOR_RED);
		i++;
	}
	return;
}

void s_printAlert(char* message){
	int i = 0;
	while(message[i]!=0){
		syscall(1000, 9, (uint32_t)&message[i], S_CHAR_COLOR_LIGHT_BROWN);
		i++;
	}
	return;
}

void s_printSuccess(char* message){
	int i = 0;
	while(message[i]!=0){
		syscall(1000, 9, (uint32_t)&message[i], S_CHAR_COLOR_GREEN);
		i++;
	}
	return;
}

void s_sleep(int time){
	syscall(1400, 1, 0, time);
	return;
}

void s_clearScreen(){
	syscall(1000, 1, 0, 0);
	return;
}

void s_move_cursor_back(){
	syscall(1000, 6, 0, 0);
	return;
}

void s_move_cursor_forward(){
	syscall(1000, 7, 0, 0);
	return;
}

void s_setBackgroundColor(char color){
	syscall(1000, 2, 0, (int)color);
	return;
}

void s_switchTimeStyle(){
	syscall(1400, 3, 0 ,0);
	return;
}

void s_shutdown(){
	syscall(1500, 1, 0, 0);
	return;
}

void s_reboot(){
	syscall(1500, 2, 0 ,0);
	return;
}

void s_scrolldown(){
	syscall(1000, 8, 0, 0);
	return;
}

void s_smb_BIOSinfo(){
	syscall(1300, 1, 0, 0);
	return;
}