/*
* Copyright 2015 Alejandro Bezdjian
* Based on code from JamesM's kernel development tutorials.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../include/system.h"

// End of kernel (from linker script)
extern uint32_t end;
// Total pages that fit in memory
static int nframes;
// This is the beggining of the stack which finishes at end.
static uint32_t pmm_stack_start;
static uint32_t pmm_stack_size;
static uint32_t pmm_stack_current;
// This is where we can start allocating pages, above the stack.
static uint32_t pmm_start;
static uint32_t pmm_current;
static uint8_t pmm_paging_active = 0;

int init_pmm(multiboot_info_t* mboot){
    if (!CHECK_BIT(mboot->flags, 6)){
      panic("Memory map not provided by GRUB.");
    }
    //print_memory_map(mboot);

    nframes = ((mboot->mem_lower + mboot->mem_upper) * 1024) / PAGE_SIZE;
    pmm_stack_size = (nframes-(end/PAGE_SIZE)) * 4; // How many ints I need to store the max possible frames.
  	// Ensure the initial page allocation location is page-aligned.
  	pmm_start = (end + pmm_stack_size + 0x1000) & PAGE_MASK;
  	pmm_current = pmm_start;
  	pmm_stack_start = pmm_start;
  	pmm_stack_current = pmm_stack_start;
  	return nframes;
}

uint32_t pmm_alloc_page(){
  	if(pmm_paging_active){
    	// Quick sanity check.
    	if (pmm_stack_current == pmm_start){
      		panic("Error: out of memory.");
    	}
    	// Pop off the stack.
    	uint32_t* stack = (uint32_t*)pmm_stack_current;
    	pmm_stack_current += sizeof(uint32_t);
    	return *stack;
  	}else{
  		uint32_t aux = pmm_start;
  		pmm_start += PAGE_SIZE;
    	return aux;
  	}
}

void pmm_free_page(uint32_t p){
  	// Ignore any page under "location", as it may contain important data initialised
  	if (p < pmm_start){
  		return;
  	}
    // We allocated enough space in for the stack to hold everything we need
    // We check only to see if something horrible happened
    if(pmm_stack_current < (pmm_stack_start-pmm_stack_size)){
      	panic("Error: Tried to free more space than available.");
    }
    pmm_stack_current -= sizeof(uint32_t);
    uint32_t* stack = (uint32_t*)pmm_stack_current;
    *stack = p;
    return;
}

void pmm_paging_stop(){
    uint32_t start, final;
    start = pmm_start;
    final = nframes*PAGE_SIZE;
    pmm_paging_active = 0;
    while(start<final){
        pmm_free_page(start);
        start += PAGE_SIZE;
    }
    return;
}

/* Prints on screen the memory map */
void print_memory_map(multiboot_info_t* mboot){
  	// unsigned long i = mboot->mmap_addr;
   //  unsigned long final = mboot->mmap_addr + mboot->mmap_length;
   //  k_printf("%s\n", "Memory map:");
   //  while (i < final){
   //      unsigned int* size = (unsigned int*) i;
   //      unsigned long* base_addr = (unsigned long*) (i + 4);
   //      unsigned long* length = (unsigned long*) (i + 12);
   //      unsigned int* type = (unsigned int*) (i + 20);
   //      k_printf("\tbase addr: 0x%x length: 0x%x type: %d\n", *base_addr, *length, *type);
   //      i += *size + 4;
   //  }
   //  k_printf("%s%x%s\n", "Total memory: 0x", ((mboot->mem_lower + mboot->mem_upper) * 1024), " uint8_ts.");
   //  k_printf("End of kernel addr: 0x%x\n", (uint32_t)&end);
    return;
}