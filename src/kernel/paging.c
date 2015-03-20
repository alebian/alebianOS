#include "../../include/system.h"

/*
 * This is the paging and physical memory manager implementation
 * It uses a stack to save all available frame addresses
 * It has 2 stacks, one for low memory frames and one for the rest
 * This is because it's not recommended to use memory below 1MB and sometimes below 16MB
 */

/* This stack stores available pages above LOW_MEMORY_TOP */
static volatile address_t frame_stack[MAX_PAGES];
static volatile int frame_stack_current = 0;
static int frame_stack_max = 0;

page_directory* page_dir = 0;

void start_paging(multiboot * mboot){
    unsigned int i, j, k, final, aux;
    unsigned int page_directory_idx, page_table_idx, phys_page_addr;

	// Check if map is provided
	if (!(mboot->flags & 0x40)){
		k_panic("GRUB failed to provide a memory map.");
	}

	print_memory_map(mboot);

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
				if(current>=LOW_MEMORY_TOP){
					push_frame(current);
					frame_stack_max++;
				}
				k++;
			}	
		}
		i += *size + 4;
	}

	// Create a page directory
	aux = pop_frame();
	page_dir = (page_directory*)aux;
	for (i = 0; i < 1024; i++){
		// Attribute: supervisor, write, not present.
		page_dir->tables[i] = 2;
	}

	// Create the first page table which will have mapped the low memory and set it as read
	page_dir->tables[0] = create_page_directory_entry()+1; // Since I'm going to map a few pages I sum 1
	page_table* current_table = (page_table*)(page_dir->tables[0]>>12);
	i = 0;
	final = LOW_MEMORY_TOP/PAGE_SIZE;
	while(i<final){
		current_table->pages[i] = ((i*PAGE_SIZE)<<12)+3;
		i++;
	}

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
			// Base address of a frame:
			address_t current = *base_addr+(k*PAGE_SIZE);
			page_directory_idx = current>>22;
			page_table_idx = (current>>12) & 0x03FF;
			phys_page_addr = current/PAGE_SIZE;
			u32int page_directory_entry = page_dir->tables[page_directory_idx];

			if(current>=LOW_MEMORY_TOP && *type==1){
				if( !(page_directory_entry & 1) ){
					// If the page table is not present we create one
					page_directory_entry = create_page_directory_entry()+1; // If we are here it means that we will add a page so it's present
					page_dir->tables[page_directory_idx] = page_directory_entry;
				}
				// We map the frame into the page table
				current_table = (page_table*)(page_directory_entry>>12);
				map_to_page_table(current, *type, page_directory_idx, current_table);
				
			}else{
				
			}

			k++;
		}
		i += *size + 4;
	}

	// Finally we enable paging
	_write_cr3((unsigned int)&(*page_dir));
	_write_cr0(_read_cr0() | 0x80000000); // Set the paging bit in CR0 to 1
	return;
}

/*
 * This function creates a new page table and returns a page directory entry
 * IT DOES NOT RETURN ANY PHYSICAL ADDRESS 
 */
unsigned int create_page_directory_entry(){
	int w;
	address_t aux = pop_frame();
	page_table* newTable = (page_table*)aux;
	for(w=0; w<1024; w++){
		// Attribute: supervisor, write, not present.
		newTable->pages[w] = 0 | 2;
	}
	aux = (aux<<12)+2;
	return aux;
}

/*
 * This function assignates a frame into a page table entry
 * Returns the type of memory it mapped
 */
unsigned int map_to_page_table(address_t addr, unsigned int type, int idx, page_table* table){
	// First we find the index of the page in the table
	int page_table_idx;
	if(addr>=1024*PAGE_SIZE){
		page_table_idx = (addr-(idx*1024*PAGE_SIZE))/PAGE_SIZE;
	}else{
		page_table_idx = addr/PAGE_SIZE;
	}
	table->pages[page_table_idx] = (addr<<12);
	if(type==1){
		// Set it to user, write, present
		table->pages[page_table_idx] += 7;
	}else{
		// Set it to supervisor, write, present
		table->pages[page_table_idx] += 3;
	}
	return type;
}



address_t pop_frame(){
  if(frame_stack_current<=0){
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
 * Prints on screen the memory map
 */
void print_memory_map(multiboot * mboot){
	unsigned int i = mboot->mmap_addr;
	unsigned int final = mboot->mmap_addr + mboot->mmap_length;
	printf("\n%s\n", "Memory map:");
	while (i < final){
		unsigned int* size = (unsigned int*) i;
		unsigned int* base_addr = (unsigned int*) (i + 4);
		unsigned int* length = (unsigned int*) (i + 12);
		unsigned int* type = (unsigned int*) (i + 20);
		printf("\tbase adr:%x length:%x type:%d\n", *base_addr, *length, *type);
		i += *size + 4;
	}
	printf("%s%d bytes\n", "Total available memory: ", ((mboot->mem_lower + mboot->mem_upper) * 1024));
	return;
}
