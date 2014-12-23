#ifndef _kc_
#define _kc_

#include "defs.h"

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access, byte zero);
size_t __write(int fd, char* buffer, size_t count);
size_t __read(int fd, char* buffer, size_t count);
void showLoadingScreen();

#endif
