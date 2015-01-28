#ifndef _kernel_
#define _kernel_

typedef struct {
	char* description;
	void(*vinit)(void);
	void(*minit)(multiboot*);
} __attribute__ ((aligned)) initializable;

void k_nice_square(void);

#endif
