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
	syscall(SYS_SOUND, 1, 0, 0); // Beep
	return;
}

void s_setKBlistener(int listener){
	syscall(SYS_EVENTS, 1, listener, 0);
	return;
}

void s_printError(char* message){
	int i = 0;
	while(message[i]!=0){
		syscall(SYS_VIDEO, 9, (uint32_t)&message[i], S_CHAR_COLOR_RED);
		i++;
	}
	return;
}

void s_printAlert(char* message){
	int i = 0;
	while(message[i]!=0){
		syscall(SYS_VIDEO, 9, (uint32_t)&message[i], S_CHAR_COLOR_LIGHT_BROWN);
		i++;
	}
	return;
}

void s_printSuccess(char* message){
	int i = 0;
	while(message[i]!=0){
		syscall(SYS_VIDEO, 9, (uint32_t)&message[i], S_CHAR_COLOR_GREEN);
		i++;
	}
	return;
}

void s_sleep(int time){
	syscall(SYS_TIMER, 1, 0, time);
	return;
}

void s_clearScreen(){
	syscall(SYS_VIDEO, 1, 0, 0);
	return;
}

void s_move_cursor_back(){
	syscall(SYS_VIDEO, 6, 0, 0);
	return;
}

void s_move_cursor_forward(){
	syscall(SYS_VIDEO, 7, 0, 0);
	return;
}

void s_setBackgroundColor(char color){
	syscall(SYS_VIDEO, 2, 0, (int)color);
	return;
}

void s_switchTimeStyle(){
	syscall(SYS_TIMER, 3, 0 ,0);
	return;
}

void s_shutdown(){
	syscall(SYS_IO, 1, 0, 0);
	return;
}

void s_reboot(){
	syscall(SYS_IO, 2, 0 ,0);
	return;
}

void s_scrolldown(){
	syscall(SYS_VIDEO, 8, 0, 0);
	return;
}

void s_smb_BIOSinfo(){
	syscall(SYS_SMBIOS, 1, 0, 0);
	return;
}

void s_setmousesensitivity(char s){
	syscall(SYS_MOUSE, 1, 0, s);
	return;
}

void s_processorinfo(){
	syscall(SYS_CPU, 1, 0, 0);
	return;
}