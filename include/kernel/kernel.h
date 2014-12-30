#ifndef _kernel_
#define _kernel_

#include "../../include/kernel/defs.h"

#define OS_PID	0
#define IDT_ENTRY_NUMBER 0xFF

#define PAGE_DIRECTORY_ADDRESS 0x9C000
#define PAGE_TABLE_ADDRESS 0x9D000
#define PAGE_SIZE 4096 // 4096 = 4kb

void setup_paging();
void setup_keyboard_buffer();
void setup_IDT_content();
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access, byte zero);
void setup_IDTR();
void setup_PIC();

#endif
