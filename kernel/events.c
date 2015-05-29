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
#include "../common.h"

static klistener keyboard_listener = &k_OSkblistener;
static clicklistener lclick_listener = &k_OSlclcklistener;
static clicklistener rclick_listener = &k_OSrclcklistener;
static clicklistener mclick_listener = &k_OSmclcklistener;

void k_KeyboardListener(){
	keyboard_listener();
	return;
}

void k_setKeyboardListener(klistener listener){
	keyboard_listener = listener;
	return;
}

void k_mouseRightClick(int x, int y){
	rclick_listener(x,y);
	return;
}

void k_setRclickListener(clicklistener listener){
	rclick_listener = listener;
	return;
}

void k_mouseLeftClick(int x, int y){
	lclick_listener(x,y);
	return;
}

void k_setLclickListener(clicklistener listener){
	lclick_listener = listener;
	return;
}

void k_mouseMidClick(int x, int y){
	mclick_listener(x,y);
	return;
}

void k_setMclickListener(clicklistener listener){
	mclick_listener = listener;
	return;
}

void k_OSkblistener(){
	get_char_from_keyboard_buffer();
	return;
}

void k_OSlclcklistener(int x, int y){
	if(y==0){
		if(x>=0 && x<k_strlen(START_LOGO)){
			if(!k_sbarmenuopened()){
				k_openStartMenu();
				return;
			}
		}
		else if(x>=72){
			change_time_style();
			return;
		}
	}
	else if(y==2){
		if(k_sbarmenuopened()){
			/* Shutdown */
			if(x>=0 && x<8){
				k_closeStartMenu();
				k_shutdown();
				return;
			}
		}
	}
	else if(y==3){
		if(k_sbarmenuopened()){
			/* Reboot */
			if(x>=0 && x<6){
				k_closeStartMenu();
				k_reboot();
				return;
			}
		}
	}
	if(k_sbarmenuopened()){
		k_closeStartMenu();
	}
	return;
}

void k_OSrclcklistener(int x, int y){
	return;
}

void k_OSmclcklistener(int x, int y){
	return;
}