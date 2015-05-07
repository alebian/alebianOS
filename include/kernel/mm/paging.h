#ifndef _paging_
#define _paging_

#define PAGE_SIZE 4096 // 4096b = 4kb
#define MAX_PAGES 1024*1024 //4GB, 4kb/page
#define KERNEL_MAX_FRAMES 1
#define PAGE_MASK 0xFFFFF000

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
    u32int present    : 1;
    u32int rw         : 1;
    u32int user       : 1; 
    u32int writet     : 1;
    u32int cache      : 1;
    u32int accessed   : 1;
    u32int dirty      : 1;
    u32int zero       : 1;
    u32int global     : 1;
    u32int avail      : 3;
    u32int frame      : 20;
} __attribute__ ((packed)) page;

typedef struct{
    page pages[1024];
} __attribute__ ((packed)) page_table;

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
	u32int present    : 1;
	u32int rw         : 1;
	u32int user       : 1;
	u32int writet     : 1;
	u32int cache      : 1;
	u32int accessed   : 1;
	u32int zero       : 1;
	u32int size       : 1;
	u32int ignored    : 1;
	u32int avail      : 3;
	u32int frame      : 20;
} __attribute__ ((packed)) page_dir_entry;

typedef struct{
    page_dir_entry tables[1024];
} __attribute__ ((packed)) page_directory;

int init_paging();

#endif