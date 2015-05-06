#include "../../include/system.h"

GDT_ENTRY gdt[GDT_SIZE];
GDT_PTR gdt_ptr;

int init_GDT(){
	gdt_ptr.limit = (sizeof(GDT_ENTRY) * GDT_SIZE) - 1;
	gdt_ptr.base  = (u32int)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

	_lgdt(&gdt_ptr);
	return GDT_SIZE;
}

void gdt_set_gate(s32int num, u32int base, u32int limit, u8int access, u8int gran){
   gdt[num].base_low    = (base & 0xFFFF);
   gdt[num].base_middle = (base >> 16) & 0xFF;
   gdt[num].base_high   = (base >> 24) & 0xFF;

   gdt[num].limit_low   = (limit & 0xFFFF);
   gdt[num].granularity = (limit >> 16) & 0x0F;

   gdt[num].granularity |= gran & 0xF0;
   gdt[num].access      = access;
   return;
}