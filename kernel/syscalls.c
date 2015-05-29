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

#include "include/system.h"
#include "../common.h"

void k_syscall(uint32_t syscall, uint32_t fd, uint32_t buffer, uint32_t edx){
	switch(syscall){
		/* Syscall: Read */
		case 3:
			sys_read(fd, buffer, edx);
			break;
		/* Syscall: Write */
		case 4:
			sys_write(fd, buffer, edx);
			break;
		/* Syscall: special video functions */
		case 1000:
			sys_video(fd, buffer, edx);
			break;
		/* Syscall: special sound functions */
		case 1100:
			sys_sound(fd, buffer, edx);
			break;
		/* Syscall: set OS listeners */
		case 1200:
			sys_listeners(fd, buffer, edx);
			break;
		/* Syscall: special SMBIOS functions */
		case 1300:
			sys_smbios(fd, buffer, edx);
			break;
		/* Syscall: special timer functions */
		case 1400:
			sys_timer(fd, buffer, edx);
			break;
		case 1500:
			sys_io(fd, buffer, edx);
			break;
		default:
			break;
	}
	return;
}

uint32_t sys_write(uint32_t fd, uint32_t buffer, uint32_t count){
	uint8_t* aux = (uint8_t*)buffer;
	uint32_t c;
	uint32_t i;
	switch(fd){
		/* 1: Video output */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		print(aux[i]);
	    		c++;
	    	}
	    	break;
	    }
	    /* Keyboard */
	    case 2:
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		add_to_keyboard_buffer(aux[c]);
	    		c++;
	    	}
	    	break;
	    default: {
	    	break;
	    }
	}
	return c;
}

uint32_t sys_read(uint32_t fd, uint32_t buffer, uint32_t count){
	uint8_t* aux = (uint8_t*)buffer;
	uint32_t c;
	uint32_t i;
	switch(fd){
		/* 1: Keyboard input */
	    case 1: {
	    	c = 0;
	    	for (i = 0; i < count; i++){
	    		aux[i] = get_char_from_keyboard_buffer();
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

void sys_video(uint32_t func, uint32_t buffer, uint32_t esp){
	uint8_t* aux = (uint8_t*)buffer;
	switch(func){
		case 1:
			clearScreen();
			break;
		case 2:
			setBackgroundColor(esp);
			break;
		case 3:
			setCharacterColor((char)esp);
			break;
		case 4:
			setAllCharacterColor((char)esp);
			break;
		case 5:
			//setMouseColor((char)esp);
			break;
		case 6:
			move_cursor_back();
			break;
		case 7:
			move_cursor_forward();
			break;
		case 8:
			scrolldown();
			break;
		case 9: 
	    	printWithColor(aux[0], (char)esp);
	    	break;
		default:
			break;
	}
	return;
}

void sys_sound(uint32_t func, uint32_t buffer, uint32_t esp){
	char* aux = (char*)buffer;
	switch(func){
		case 1:
			beep();
			break;
		case 2:
			play_sound(esp);
			break;
		case 3:
			playNote(aux, esp);
			break;
		default:
			break;
	}
	return;
}

void sys_listeners(uint32_t func, uint32_t listener, uint32_t esp){
	switch(func){
		case 1:
			k_setKeyboardListener((klistener)listener);
			break;
		case 2:
			k_setLclickListener((clicklistener)listener);
			break;
		case 3:
			k_setRclickListener((clicklistener)listener);
			break;
		case 4:
			k_setMclickListener((clicklistener)listener);
			break;
		default:
			break;
	}
	return;
}

void sys_smbios(uint32_t func, uint32_t buffer, uint32_t esp){
	switch(func){
		case 1:
			smb_BIOSinfo();
			break;
		default:
			break;
	}
	return;
}

void sys_timer(uint32_t func, uint32_t buffer, uint32_t count){
	uint8_t* ans = (uint8_t*)buffer;
	uint32_t i, j;
	char* aux;
	switch(func){
		case 1:
			sleep(count);
			break;
		case 2: {
			i = 0;
			j = 0;
			aux = get_current_time();
			while( i<13 ){
				if(aux[i] != ' '){
					ans[j++] = aux[i];
				}
				i++;
			}
			ans[j] = 0;
			break;
		}
		case 3:
			change_time_style();
			break;
		case 4: {
			i = 0;
			aux = get_current_time();
			while( i<13 ){
				ans[i] = aux[i];
				i++;
			}
			ans[i] = 0;
			break;
		}
		default:
			break;
	}
	return;
}

void sys_io(uint32_t func, uint32_t buffer, uint32_t esp){
	switch(func){
		case 1:
			k_shutdown();
			break;
		case 2:
			k_reboot();
			break;
		default:
			break;
	}
	return;
}
