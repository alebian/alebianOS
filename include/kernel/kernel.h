#ifndef _kernel_
#define _kernel_

typedef struct {
	char* description;
	void(*vinit)(void);
	void(*minit)(multiboot*);
} __attribute__ ((aligned)) initializable;

void kmain(multiboot*, int);
void k_set_loading_screen(void);
void k_nice_square(void);
void k_turnOn(void);
void k_turnOff(void);
int k_isOn(void);

#endif
