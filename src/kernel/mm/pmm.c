/* Based on the JamesM tutorials */

#include "../../../include/system.h"

// End of kernel (from linker script)
extern u32int end;
// Total pages that fit in memory
static int nframes;
// This is the beggining of the stack which finishes at end.
static u32int pmm_stack_start;
static u32int pmm_stack_size;
static u32int pmm_stack_current;
// This is where we can start allocating pages, above the stack.
static u32int pmm_start;
static u32int pmm_current;
static byte pmm_paging_active = 0;

int init_pmm(multiboot_info_t* mboot){
	if (!CHECK_BIT(mboot->flags, 6)){
		panic("Memory map not provided by GRUB.");
	}
	print_memory_map(mboot);

	nframes = ((mboot->mem_lower + mboot->mem_upper) * 1024) / PAGE_SIZE;
	pmm_stack_size = (nframes-(end/PAGE_SIZE)) * 4; // How many ints I need to store the max possible frames.
  	// Ensure the initial page allocation location is page-aligned.
  	pmm_start = (end + pmm_stack_size + 0x1000) & PAGE_MASK;
  	pmm_current = pmm_start;
  	pmm_stack_start = pmm_start;
  	pmm_stack_current = pmm_stack_start;
  	return nframes;
}

u32int pmm_alloc_page(){
  	if(pmm_paging_active){
    	// Quick sanity check.
    	if (pmm_stack_current == pmm_start){
      		panic("Error: out of memory.");
    	}
    	// Pop off the stack.
    	u32int* stack = (u32int*)pmm_stack_current;
    	pmm_stack_current += sizeof(u32int);
    	return *stack;
  	}else{
  		u32int aux = pmm_start;
  		pmm_start += PAGE_SIZE;
    	return aux;
  	}
}

void pmm_free_page(u32int p){
  	// Ignore any page under "location", as it may contain important data initialised
  	if (p < pmm_start){
  		return;
  	}
    // We allocated enough space in for the stack to hold everything we need
    // We check only to see if something horrible happened
    if(pmm_stack_current < (pmm_stack_start-pmm_stack_size)){
      	panic("Error: Tried to free more space than available.");
    }
    pmm_stack_current -= sizeof(u32int);
    u32int* stack = (u32int*)pmm_stack_current;
    *stack = p;
    return;
}

void pmm_paging_stop(){
	u32int start, final;
	start = pmm_start;
	final = nframes*PAGE_SIZE;
	pmm_paging_active = 0;
	while(start<final){
		pmm_free_page(start);
		start += PAGE_SIZE;
	}
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