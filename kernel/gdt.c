/*
* Copyright 2015 Alejandro Bezdjian
* Based on code from Bran's kernel development tutorials.
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

#include "include/system.h"

GDT_ENTRY gdt[GDT_SIZE];
GDT_PTR gdt_ptr;

int init_GDT(){
	gdt_ptr.limit = (sizeof(GDT_ENTRY) * GDT_SIZE) - 1;
	gdt_ptr.base  = (uint32_t)&gdt;

	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

	_lgdt(&gdt_ptr);
	return GDT_SIZE;
}

void gdt_set_gate(sint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran){
   gdt[num].base_low    = (base & 0xFFFF);
   gdt[num].base_middle = (base >> 16) & 0xFF;
   gdt[num].base_high   = (base >> 24) & 0xFF;

   gdt[num].limit_low   = (limit & 0xFFFF);
   gdt[num].granularity = (limit >> 16) & 0x0F;

   gdt[num].granularity |= gran & 0xF0;
   gdt[num].access      = access;
   return;
}