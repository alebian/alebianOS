#ifndef _syscalls_
#define _syscalls_

#include "kernel.h"

void k_syscall(dword, dword, dword, dword);
dword __write(int, unsigned char*, dword);
dword __read(int, unsigned char*, dword);

#endif