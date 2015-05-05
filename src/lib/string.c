#include "../../include/lib/string.h"

void* memcpy(void* s, const void* ct, int n){
    int i;
    for (i = 0; i < n; i++){
        *((byte*)s + i) = *((byte*)ct + i);
    }
    return s;
}

void* memset(void* s, int c, int n){
    int i;
    for (i = 0; i < n; i++){
        *((byte*)s + i) = (byte)c;
    }
    return s;
}

int memcmp(const void *str1, const void *str2, int n){
    int ans = 1;
    int i;
    for(i = 0 ; i < n ; i++){
        if(*((byte*)str1 + i) != *((byte*)str2 + i)){
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
