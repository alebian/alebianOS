#include "../../../include/system.h"

page_directory* page_dir = (page_directory*)0x400000;
page_table* first_table = (page_table*)0x401000; // First 4MB kernel land

void start_paging(multiboot_info_t* mboot){
	int i;

	if (!CHECK_BIT(mboot->flags, 6)){
		k_panic("Memory map not provided by GRUB.");
	}
	//print_memory_map(mboot);

	/* Initialize the page directory */
	for(i = 0; i < 1024; i++){
		page_dir_entry entry;
		entry.present = 1;
		entry.rw = 1;
		entry.user = 0;
	    entry.writet = 0;
	    entry.cache = 0;
	    entry.accessed = 0;
	    entry.zero = 0;
	    entry.size = 0;
	    entry.ignored = 0;
	    entry.avail = 0;
		entry.frame = 0;
	    page_dir->tables[i] = entry; // attributes: supervisor level, read/write, not present.
	}

	/* Identity paging for the first 4MB (kernel land) */
	for(i = 0; i < 1024; i++){
		page pg;
		pg.present = 1;
		pg.rw = 1;
		pg.user = 0;
	    pg.writet = 0;
	    pg.cache = 0;
	    pg.accessed = 0;
	    pg.dirty = 0;
	    pg.zero = 0;
	    pg.global = 0;
	    pg.avail = 0;
		pg.frame = i;
	    first_table->pages[i] = pg;
	}

	page_dir->tables[0].present = 1;
	page_dir->tables[0].frame = (unsigned int)first_table/4096;

	printf("%s%x\n", "Page table addess: ", first_table);
	printf("%s%x\n", "Page table addess frame: ", (unsigned int)first_table/4096);
	printf("%s%x\n", "Page directory first entry: ", page_dir->tables[0]);

	/* Finally we enable paging */
	_write_cr3((unsigned int)page_dir);
	_write_cr0(_read_cr0() | 0x80000000); // Set the paging bit in CR0 to 1
	return;
}

/*
 * Prints on screen the memory map
 */
void print_memory_map(multiboot_info_t* mboot){
	unsigned long i = mboot->mmap_addr;
	unsigned long final = mboot->mmap_addr + mboot->mmap_length;
	printf("%s\n", "Memory map:");
	while (i < final){
		unsigned int* size = (unsigned int*) i;
		unsigned long* base_addr = (unsigned long*) (i + 4);
		unsigned long* length = (unsigned long*) (i + 12);
		unsigned int* type = (unsigned int*) (i + 20);
		printf("\tbase addr: 0x%x length: 0x%x type: %d\n", *base_addr, *length, *type);
		i += *size + 4;
	}
	return;
}
