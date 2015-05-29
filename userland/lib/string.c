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

#include "../include/lib/string.h"
#include "../../common.h"

void* memcpy(void* s, const void* ct, int n){
    int i;
    for (i = 0; i < n; i++){
        *((uint8_t*)s + i) = *((uint8_t*)ct + i);
    }
    return s;
}

void* memset(void* s, int c, int n){
    int i;
    for (i = 0; i < n; i++){
        *((uint8_t*)s + i) = (uint8_t)c;
    }
    return s;
}

int memcmp(const void *str1, const void *str2, int n){
    int ans = 1;
    int i;
    for(i = 0 ; i < n ; i++){
        if(*((uint8_t*)str1 + i) != *((uint8_t*)str2 + i)){
            ans = 0;
            break;
        }
    }
    return ans;
}

void* memsetw(void* s, int c, int n){
    int i;
    for (i = 0; i < n; i++){
        *((short*)s + i) = (unsigned short) c;
    }
    return s;
}

char* strcpy(char* s, const char* ct){
    char* aux = s;
    while (*ct){
        *aux++ = *ct++;
    }
    return s;
}

void reverse(char* s){
    int c, i, j;
    for(i = 0, j = strlen(s)-1; i < j; i++, j--){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int strlen(char* s){
    int i=0;
    while( s[i] != 0 ){
        i++;
    }
    return i;
}

int strcmp(const char* cs, const char* ct){
    for(; *cs == *ct; cs++, ct++)
        if(*cs == '\0')
            return 0;
    return *cs - *ct;
}
