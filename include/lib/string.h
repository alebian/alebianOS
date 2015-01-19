#ifndef _string_
#define _string_

#include "../kernel/defs.h"

void * memcpy(void *, const void *, dword);
void * memset(void *, int, dword);
void * memsetw(void *, int, dword);
char * strcpy(char *, const char *);
void reverse(char *);
int strlen(char *);
int strcmp(const char *, const char *);

#endif
