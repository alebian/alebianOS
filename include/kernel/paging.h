#ifndef _paging_
#define _paging_

#include "../multiboot.h"

typedef unsigned int address_t;

#define PAGE_SIZE 4096 // 4096b = 4kb
#define MAX_PAGES 1024*1024 //4GB, 4kb/page
/* 
 * Here we set the low memory in bytes 
 * In this case we set low memory to be everything below 1MB
 */
#define LOW_MEMORY_TOP  1*1024*1024

typedef struct{
    // Bit 0: P -> Present, if set the page is present in memory
    // Bit 1: R -> Read, if set the page can be written
    // Bit 2: U -> User, if clear supervisor (kernel) level
    // Bit 3: W -> Write Through, always 0
    // Bit 4: D -> Cache Disabled, always 0
    // Bit 5: A -> Accessed, if set some page has been accessed
    // Bit 6: 0 -> Zero, always 0
    // Bit 7: S -> Size, if set pages are 4MB
    // Bit 8: G -> Ignored
    // Bit 9-11: Available, free to use by OS
    // Bit 12-31: Page table address
    unsigned int tables[1024];
} __attribute__ ((packed)) page_directory;

typedef struct{
    // Bit 0: P -> Present, if set the page is present in memory
    // Bit 1: R -> Read, if set the page can be written
    // Bit 2: U -> User, if clear supervisor (kernel) level
    // Bit 3: W -> Write Through, always 0
    // Bit 4: C -> Cache Disabled, always 0
    // Bit 5: A -> Accessed, if set the page has been accessed
    // Bit 6: D -> Dirty, if set the page has been written
    // Bit 7: 0 -> Zero, always 0
    // Bit 8: G -> Global, always 0
    // Bit 9-11: Available, free to use by OS
    // Bit 12-31: Page table address
	unsigned int pages[1024];
} __attribute__ ((packed)) page_table;

void start_paging(multiboot*);
unsigned int create_page_directory_entry(void);
unsigned int map_to_page_table(address_t, unsigned int, int, page_table*);
address_t pop_frame(void);
void push_frame(address_t);
void print_memory_map(multiboot*);

#endif