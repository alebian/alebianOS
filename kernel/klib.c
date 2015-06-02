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
#include "../userland/include/lib/stdarg.h"
#include "../userland/include/lib/stdlib.h"

void k_printf(char* fmt, ...){
	va_list ap;
	va_start(ap, fmt);
	char *p, *sval;
	int ival;
	static char s[80];
	for(p = fmt; *p; p++){
	    if(*p != '%'){
	    	k_putchar(*p);
	        continue;   
	    }
	    switch(*++p){
	        case 'd':
	            ival = va_arg(ap, int);
	            itoa(s, ival, 10);
	            k_puts(s);
	            break;
	        case 'x':
	            ival = va_arg(ap, int);
	            itoa(s, ival, 16);
	            k_puts(s);
	            break;
	        case 'c':
	            ival = va_arg(ap, int);          
	            k_putchar(ival);
	            break;
	        case 's':
	            for(sval = va_arg(ap, char*); *sval; sval++){
	                k_putchar(*sval);
	            }
	            break;
	        default:
	            k_putchar(*p);
	        break;
	    }
	}
	va_end(ap);
	return;
}

void k_putchar(char c){
	print(c);
	return;
}

int k_puts(char* stream){
    while(*stream){
        print(*stream);
        stream++;   
    }
    return 0;
}

void k_printWithColor(char* s, char color){
	int i = 0;
	while(s[i] != 0){
		printWithColor(s[i], color);
		i++;
	}
	return;
}

void k_printWarning(char* s){
	k_printWithColor(s, CHAR_COLOR_LIGHT_BROWN);
	return;
}

void k_printError(char* s){
	k_printWithColor(s, CHAR_COLOR_RED);
	return;
}

void k_printSuccess(char* s){
	k_printWithColor(s, CHAR_COLOR_GREEN);
	return;
}

void k_disable(){
	k_disableSbar();
	k_disableMouse();
	return;
}

void k_shutdown(){
	k_disable();
	k_shutdownScreen();
	// Just to appreciate the art
	_Sti();
	sleep(30);
	if(ACPIinitialized()){
		acpiPowerOff();
	}else{
		_Cli();
		while(1){}
	}
	return;
}

void k_reboot(){
	k_disable();
	k_rebootanimation();
	_outb(0x64, 0xFE);
	return;
}

void* k_memcpy(void* s, const void* ct, int n){
    int i;
    for (i = 0; i < n; i++){
        *((uint8_t*)s + i) = *((uint8_t*)ct + i);
    }
    return s;
}

void* k_memset(void* s, int c, int n){
    int i;
    for (i = 0; i < n; i++){
        *((uint8_t*)s + i) = (uint8_t)c;
    }
    return s;
}

int k_memcmp(const void *str1, const void *str2, int n){
    int i;
    for(i = 0 ; i < n ; i++){
        if(*((uint8_t*)str1 + i) != *((uint8_t*)str2 + i)){
            return *((uint8_t*)str1 + i) - *((uint8_t*)str2 + i);
        }
    }
    return 0;
}

int k_strlen(char* s){
    int i=0;
    while( s[i] != 0 ){
        i++;
    }
    return i;
}