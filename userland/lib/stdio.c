/*
* Copyright 2014 Alejandro Bezdjian
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

#include "../../common/stdarg.h"
#include "../../common/common.h"
#include "../include/lib/stdio.h"
#include "../include/syscall.h"

/* Input */

int scanf(char* fmt, ...){
    va_list ap;
    char *p, *sval, *cval;
    int *ival;
    //char fp;
    int read = 0,i = 0,c ,k ,j = 0;
    va_start(ap, fmt);
    static char s[MAX_BUFFER];
    while((c = getchar()) != '\n' && i < MAX_BUFFER-1){
        if(c != 0){
            if(c == '\b' && i >= 1){
                i--;
                putchar(c);
            }
            if(c != '\b' && c!='\n'){
                if(i < MAX_BUFFER - 2){
                    s[i] = c;
                    i++;
                    putchar(c); 
                }
                if(i == MAX_BUFFER - 1){
                    syscall(1100, 1, 0, 0);
                }
            }
        }
    }
    s[i] = '\0';
    i = 0;
    for (p = (char*)fmt; *p; p++) {
        static char aux [MAX_BUFFER] = {0};
        if(*p != '%'){
            c = s[j++];
            if(c =='\0' || *p != c)
                return read;
            continue;
        }
        switch (*++p){
            case 'd':
                i = 0;
                ival = va_arg(ap,int*);
                while(s[j] != '\0' && isdigit(s[j])){
                    aux[i++] = s[j++];
                }
                aux[i] = '\0';
                if(aux[0] == '\0'){
                    return read;
                }
                *ival = atoi(aux);
                read++;
                break;
            case 's':
                i = 0;
                sval = va_arg(ap, char*);
                while(s[j] != '\0' && s[j] != ' '){
                    aux[i++] = s[j++];
                }
                aux[i] = '\0';
                if(aux[0] == '\0'){
                    return read;
                }
                k = 0;
                while(aux[k] != '\0'){
                    sval[k] = aux[k];
                    k++;
                }
                sval[k] = '\0';
                read++;
                break;
            case 'c':
                cval = va_arg(ap, char*);
                c = s[j++];
                if(c == '\0'){
                    return read;
                }
                *cval = c;
                read++;
                break;
            case 'x':
                i = 0;
                ival = va_arg(ap, int*);
                while(s[j] != '\0' && (isdigit(s[j]) || isxdigit(s[j]))){
                    aux[i] = s[j];
                    i++;
                    j++;
                }
                aux[i] = '\0';
                if(aux[0] == '\0'){
                    return read;
                }
                read++;
                break;
            default:
                c = s[j++];
                if(!(c != EOF && c != '\n' && *p == c)){
                    return read;
                }
            break;
        }
    }
    va_end(ap);
    return read;
}

unsigned char getchar(void){
    unsigned char c;
    syscall(3, 1, (uint32_t)&c, 1);
    return c;
}

/* Output */

int printf(char* fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    char *p, *sval;
    int ival;
    static char s[MAX_STRING_LENGTH];
    for(p = fmt; *p; p++){
        if(*p != '%'){
            putchar(*p);
            continue;   
        }
        switch(*++p){
            case 'd':
                ival = va_arg(ap, int);
                itoa(s, ival, 10);
                puts(s);
                break;
            case 'x':
                ival = va_arg(ap, int);
                itoa(s, ival, 16);
                puts(s);
                break;
            case 'c':
                ival = va_arg(ap, int);          
                putchar(ival);
                break;
            case 's':
                for(sval = va_arg(ap, char*); *sval; sval++){
                    putchar(*sval);
                }
                break;
            default:
                putchar(*p);
            break;
        }
    }
    va_end(ap);
    return 0;
}

int putchar(unsigned char c){
    syscall(4, 1, (uint32_t)&c, 1);
    return 1;
}

int puts(char* stream){
    while(*stream){
        putchar(*stream);
        stream++;   
    }
    return 0;
}