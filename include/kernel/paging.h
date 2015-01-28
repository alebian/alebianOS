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

typedef struct {
	unsigned int present    	: 1;	// If set the page is present in memory
    unsigned int rw         	: 1;	// If clear, Read-only
    unsigned int user       	: 1;	// If clear, Supervisor level only
    unsigned int writethrough 	: 1;	// Always 0
    unsigned int cachedisabled	: 1;	// Always 0
    unsigned int accessed   	: 1;	// If set, the page has been accessed
    unsigned int zero	      	: 1;	// Always 0
    unsigned int size			: 1;	// If set pages are 4MB
    unsigned int ignored		: 1;
    unsigned int available     	: 3;	// Free to use bits
    unsigned int frame      	: 20;	// Page table 4kb aligned address
} __attribute__ ((packed)) page_directory_entry;

typedef struct{
	page_directory_entry tables[1024];
	// page_table* tables[1024];
	// unsigned int tables_physical[1024];
	// unsigned int physical_addr; // Physical address of tables_physical
} __attribute__ ((packed)) page_directory;

typedef struct {
	unsigned int present    	: 1;	// If set the page is present in memory
    unsigned int rw         	: 1;	// If clear, Read-only
    unsigned int user       	: 1;	// If clear, Supervisor level only
    unsigned int writethrough 	: 1;	// Always 0
    unsigned int cachedisabled	: 1;	// Always 0
    unsigned int accessed   	: 1;	// If set, the page has been accessed
    unsigned int dirty      	: 1;	// If set, the page has been written
    unsigned int zero			: 1;	// Always 0
    unsigned int global			: 1;	// Always 0
    unsigned int available     	: 3;	// Free to use bits
    unsigned int frame      	: 20;	// Physical page address
} __attribute__ ((packed)) page_table_entry;

typedef struct{
	page_table_entry pages[1024];
} __attribute__ ((packed)) page_table;


void start_paging(multiboot*);
void set_page(page_directory*, int, int, unsigned int, int, int);
void create_page_table(page_directory*, int);
address_t pop_frame(void);
void push_frame(address_t);
void print_memory_map(multiboot*);

#endif