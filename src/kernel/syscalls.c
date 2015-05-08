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

#include "../../include/system.h"

void k_syscall(dword syscall, dword ebx, dword ecx, dword edx){
	switch(syscall){
		/* Syscall: Read */
		case 3:
			__read(ebx, (unsigned char*)ecx, edx);
			break;
		/* Syscall: Write */
		case 4:
			__write(ebx, (unsigned char*)ecx, edx);
			break;
		default:
			break;
	}
	return;
}

dword __write(int fd, unsigned char* buffer, dword count){
	dword c;
	int i;
	switch(fd){
		/* 1: Video output */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		print(buffer[i], getCharacterColor());
	    		c++;
	    	}
	    	break;
	    }
	    /* Keyboard */
	    case 2:
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		add_to_keyboard_buffer(buffer[c]);
	    		c++;
	    	}	
	    default: {
	    	break;
	    }
	}
	return c;
}

dword __read(int fd, unsigned char* buffer, dword count){
	dword c;
	int i;
	switch(fd){
		/* 1: Keyboard input */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		buffer[i] = get_char_from_keyboard_buffer();
	    		c++;
	    	}
	    	break;
	    }
	    default: {
	    	break;
	    }
	}
	return c;
}