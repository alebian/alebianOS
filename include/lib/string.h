#ifndef _string_
#define _string_

#include "../kernel/defs.h"

void * memcpy(void * s, const void * ct, size_t n);
char * strcpy(char * s, const char * ct);
void reverse(char * s);
int strlen(char * s);
int strcmp(const char * cs, const char * ct);

#endif
