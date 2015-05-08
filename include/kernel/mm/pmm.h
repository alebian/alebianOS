//         Based on code from Bran's kernel development tutorials.

#ifndef _pmm_
#define _pmm_

#include "../../kernel/kernel.h"
#include "../../multiboot.h"

int init_pmm(multiboot_info_t*);
u32int pmm_alloc_page(void);
void pmm_free_page(u32int);
void print_memory_map(multiboot_info_t*);
void pmm_paging_stop(void);

#endif