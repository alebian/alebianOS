#ifndef _paging_
#define _paging_

#include "../../multiboot.h"

void start_paging(multiboot_info*);
void print_memory_map(multiboot_info*);

#endif