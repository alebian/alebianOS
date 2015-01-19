#ifndef _memory_
#define _memory_

#include "../multiboot.h"

#define OS_PID	0
#define IDT_ENTRY_NUMBER 0xFF

#define PAGE_DIRECTORY_ADDRESS 0x9C000
#define PAGE_TABLE_ADDRESS 0x9D000
#define PAGE_SIZE 4096 // 4096b = 4kb

void start_paging(void);
void start_memory_manager(multiboot *);

#endif