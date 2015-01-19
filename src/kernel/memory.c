#include "../../include/system.h"

static unsigned int memory_size;
static unsigned int pages;

void start_memory_manager(multiboot * mboot){
	// Check if map is provided
	if (!(mboot->flags & 0x40)){
		k_panic("GRUB failed to provide a memory map.");
	}

	printf("%s\n", "");
	int i = mboot->mmap_addr;
	int free = 0;
	int final = mboot->mmap_addr + mboot->mmap_length;
	while (i < final){
		int *size = (int *) i;
		int *base_addr = (int *) (i + 4);
		int *length = (int *) (i + 12);
		int *type = (int *) (i + 20);

		printf("%d %d %d\n", *base_addr, *length, *type);
		if(*type == 1){
			free+=*length;
		}
		i += *size + 4;
	}
	printf("%s%d\n", "Available memory: ", free);
	printf("%s%d\n", "Available memory: ", free/1024);
	printf("%s%d\n", "Available memory: ", (free/1024)/1024);

	unsigned int memory_size_mb = mboot->mem_upper / 1024 + 2;
	// Memory in bytes
	memory_size = memory_size_mb * 1024 * 1024;
	printf("\nSize of memory: %d KB, %d MB\n", memory_size, memory_size_mb);
	pages = memory_size / PAGE_SIZE;
	start_paging();
	return;
}

void start_paging(){
	unsigned long *page_directory = (unsigned long *) PAGE_DIRECTORY_ADDRESS;
	unsigned long *page_table = (unsigned long *) PAGE_TABLE_ADDRESS;
	unsigned long address=0;
	int i;
	/* First 4MB of memory */
	for(i=0; i<1024; i++){
		page_table[i] = address | 3; // Attribute set to: supervisor level, read/write, present (011 in binary)
		address = address + PAGE_SIZE;
	};
	/* Fill the first entry of the page directory */
	page_directory[0] = PAGE_TABLE_ADDRESS;
	page_directory[0] = page_directory[0] | 3;
	/* Fill the rest of the page directory */
	for(i=1; i<1024; i++){
		page_directory[i] = 0 | 2; // Attribute set to: supervisor level, read/write, not present (010 in binary)
	};
	_write_cr3(PAGE_DIRECTORY_ADDRESS); 
	_write_cr0(_read_cr0() | 0x80000000); // Set the paging bit in CR0 to 1
};