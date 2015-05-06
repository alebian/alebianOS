#include "../../../include/system.h"

extern u32int end;

page_directory* page_dir;
int nframes;

void start_paging(multiboot_info_t* mboot){
	unsigned int diraddr, tableaddr;
	int i, dentries, aux;
	int j = 0;
	
	if (!CHECK_BIT(mboot->flags, 6)){
		k_panic("Memory map not provided by GRUB.");
	}
	print_memory_map(mboot);

	/******************************************************/
	/* WE SHOULD USE MALLOC BUT IT IS NOT IMPLEMENTED YET */
	/*    Arbitrary address until malloc is implemented   */
	diraddr = (u32int)&end;
	/******************************************************/
	page_dir = (page_directory*)diraddr;

	/* Initialize the page directory */
	for(i = 0; i < 1024; i++){
		page_dir_entry entry;
		entry.present = 1;
		entry.rw = 1;
		if(j < KERNEL_MAX_FRAMES){
			entry.user = 0;
		}else{
			entry.user = 1;
		}
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

	/* We will use Identity paging according to the information received in the multiboot. */
	nframes = ((mboot->mem_lower + mboot->mem_upper) * 1024) / PAGE_SIZE;
	dentries = (nframes/1024)+1;

	/* Initiate all the needed page tables */
	for(i = 0 ; i < dentries ; i++){
		/* The tables will be physically contiguous to the page directory. */
		tableaddr = diraddr + ((i+1)*0x1000);
		page_table* table = (page_table*) tableaddr;
		aux = (i*1024);
		for(j = 0; j < 1024; j++){
			page pg;
			pg.present = 1;
			pg.rw = 1;
			if(j < KERNEL_MAX_FRAMES){
				pg.user = 0;
			}else{
				pg.user = 1;
			}
		    pg.writet = 0;
		    pg.cache = 0;
		    pg.accessed = 0;
		    pg.dirty = 0;
		    pg.zero = 0;
		    pg.global = 0;
		    pg.avail = 0;
			pg.frame = aux+j;
		    table->pages[j] = pg;
		}
		page_dir->tables[i].present = 1;
		page_dir->tables[i].frame = (unsigned int)tableaddr/PAGE_SIZE;
	}

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
	printf("%s%x%s\n", "Total memory: 0x", ((mboot->mem_lower + mboot->mem_upper) * 1024), " bytes.");
	printf("End of kernel addr: 0x%x\n", (u32int)&end);
	return;
}
