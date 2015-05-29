#ifndef _syscall_
#define _syscall_

#define SYS_READ	3
#define	SYS_WRITE 	4

void syscall(int, int, int, int);

#endif