#ifndef _kernel_
#define _kernel_

#define OS_PID	0
#define IDT_ENTRY_NUMBER 0xFF 

void setup_IDT_content();
void setup_IDTR();

#endif
