#include "../../include/system.h"

/*
 * This is the paging and physical memory manager implementation
 * It uses a stack to save all available frame addresses
 * It has 2 stacks, one for low memory frames and one for the rest
 * This is because it's not recommended to use memory below 1MB and sometimes below 16MB
 */

/* This stack stores available pages above LOW_MEMORY_TOP */
static volatile address_t frame_stack[MAX_PAGES];
static volatile address_t frame_stack_current = 0;

page_directory* kernel_directory = 0;
page_directory* current_directory = 0;

void start_paging(multiboot * mboot){
    unsigned int i, j, k, final;

	// Check if map is provided
	if (!(mboot->flags & 0x40)){
		k_panic("GRUB failed to provide a memory map.");
	}

	//print_memory_map(mboot);

	// Iterate over memory map to push available frames into the stack
	i = mboot->mmap_addr;
	final = mboot->mmap_addr + mboot->mmap_length;
	while (i < final){
		unsigned int* size = (unsigned int*) i;
		unsigned int* base_addr = (unsigned int*) (i + 4);
		unsigned int* length = (unsigned int*) (i + 12);
		unsigned int* type = (unsigned int*) (i + 20);
		if(*type==1){
			j = *length/PAGE_SIZE; // We make sure that we won't allocate a page with less than 4kb
			k = 0;
			while(k<j){
				address_t current = *base_addr+(k*PAGE_SIZE);
				push_frame(current);
				k++;
			}	
		}
		i += *size + 4;
	}

	// Create a page directory
	kernel_directory = (page_directory*)pop_frame();
    current_directory = kernel_directory;
    // Clear the directory
    memset(&kernel_directory, 0, 4*1024);

	// Iterate over memory map AGAIN to load the page directory and create tables
	i = mboot->mmap_addr;
	final = mboot->mmap_addr + mboot->mmap_length;
	while (i < final){
		unsigned int* size = (unsigned int*) i;
		unsigned int* base_addr = (unsigned int*) (i + 4);
		unsigned int* length = (unsigned int*) (i + 12);
		unsigned int* type = (unsigned int*) (i + 20);
		j = *length/PAGE_SIZE; // We make sure that we won't allocate a page with less than 4kb
		k = 0;
		while(k<j){
			address_t current = *base_addr+(k*PAGE_SIZE);
			// Find the page table index
    		u32int page_table_idx = current / PAGE_SIZE;
    		// Find the page directory index
    		u32int dir_idx = page_table_idx / 1024;
    		if(*type == 1){
				set_page(kernel_directory, dir_idx, page_table_idx, current, 1, 1);
			}else{
				set_page(kernel_directory, dir_idx, page_table_idx, current, 0, 0);
			}
			k++;
		}
		i += *size + 4;
	}

	// Finally we enable paging
	_write_cr3((unsigned int)&(*current_directory));
	_write_cr0(_read_cr0() | 0x80000000); // Set the paging bit in CR0 to 1

	return;
}

/*
 * Creates a new page table if available is not set
 * If available is set, it does nothing
 */
void set_page(page_directory* dir, int dir_idx, int table_idx, unsigned int address, int write, int user){
	if(dir->tables[dir_idx].available == 0){
		// Page table not exists
		create_page_table(dir, dir_idx);
	}
	page_table_entry* aux = &(((page_table*)((unsigned int)dir->tables[dir_idx].frame))->pages[table_idx]);
	aux->present = 1;
	aux->rw = write;
	aux->user = user;
	aux->writethrough = 0;
	aux->cachedisabled = 0;
	aux->accessed = 0;
	aux->dirty = 0;
	aux->zero = 0;
	aux->global = 0;
	aux->available = 1;
	aux->frame = address;
	return;
}

void create_page_table(page_directory* dir, int dir_idx){
	dir->tables[dir_idx].present = 1;
	dir->tables[dir_idx].rw = 1;
	dir->tables[dir_idx].user = 1;
	dir->tables[dir_idx].writethrough = 0;
	dir->tables[dir_idx].cachedisabled = 0;
	dir->tables[dir_idx].accessed = 0;
	dir->tables[dir_idx].zero = 0;
	dir->tables[dir_idx].size = 0;
	dir->tables[dir_idx].available = 1;
	unsigned int aux = pop_frame();
	dir->tables[dir_idx].frame = aux;
	memset((page_table*)aux, 0, 4*1024);
    return;
}

address_t pop_frame(){
  if(!frame_stack_current){
  	/*
  	 * We could return a low memory address
  	 * Future implementation of virtual memory should swap a page to disk
  	 */
    k_panic("Out of physical memory.");
  }
  return frame_stack[--frame_stack_current];
}

void push_frame(address_t paddr){
  frame_stack[frame_stack_current++] = paddr;
  return;
}

/*
 * Prints on screen memory data that might be useful 
 * for testing (memory map, size, etc)
 */
void print_memory_map(multiboot * mboot){
	unsigned int i = mboot->mmap_addr;
	unsigned int final = mboot->mmap_addr + mboot->mmap_length;
	unsigned int available = 0;
	unsigned int pages = 0;
	printf("\n%s\n", "Memory map:");
	while (i < final){
		unsigned int* size = (unsigned int*) i;
		unsigned int* base_addr = (unsigned int*) (i + 4);
		unsigned int* length = (unsigned int*) (i + 12);
		unsigned int* type = (unsigned int*) (i + 20);
		printf("\tbase adr:%x length:%x type:%d\n", *base_addr, *length, *type);
		if(*type == 1){
			available += *length;
			pages += *length/PAGE_SIZE;
		}
		i += *size + 4;
	}
	printf("\n%s%d bytes\n", "Total memory:                      ", ((mboot->mem_lower + mboot->mem_upper) * 1024));
	printf("%s%d bytes\n", "Available memory (type 1):         ", available);
	printf("%s%d bytes\n", "Usable memory (mapped into pages): ", pages*PAGE_SIZE);
	printf("%s%d\n\n", "Free pages: ", pages);
	return;
}
