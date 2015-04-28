#ifndef _string_
#define _string_

#include "../kernel/kernel.h"

void* memcpy(void*, const void*, int);
void* memset(void*, int, int);
void* memsetw(void*, int, int);
void* memsetw(void*, int, int);
char* strcpy(char*, const char*);
void reverse(char*);
int strlen(char*);
int strcmp(const char*, const char*);

#endif
