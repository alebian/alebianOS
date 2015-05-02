#include "../../../include/system.h"

extern u32int end;

void start_paging(multiboot_info_t* mboot){
	if (!CHECK_BIT(mboot->flags, 6)){
		k_panic("Memory map not provided by GRUB.");
	}
	print_memory_map(mboot);
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
