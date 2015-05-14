/*
* Copyright 2015 Alejandro Bezdjian
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

#include "../../../include/system.h"

page_directory* page_dir;

int init_paging(){
	u32int diraddr, tableaddr;
	int i;

	diraddr = pmm_alloc_page();
	page_dir = (page_directory*)diraddr;

	/* Initialize the page directory */
	for(i = 0; i < 1024; i++){
	    page_dir->tables[i] = set_page_dir_entry(0, 0, 0, 0);
	}

	/* We will use Identity paging for the first 4MB */
	tableaddr = pmm_alloc_page();
	page_table* table = (page_table*)tableaddr;
	for(i = 0; i < 1024; i++){
	    table->pages[i] = set_page_table_entry(1, 1, 0, i);
	}
	page_dir->tables[0] = set_page_dir_entry(1, 1, 0, FRAME(tableaddr));

	/* Finally we enable paging */
	_write_cr3((unsigned int)page_dir);
	_write_cr0(_read_cr0() | 0x80000000); // Set the paging bit in CR0 to 1
	/* Tell the PMM that we don't need any more pages */
	pmm_paging_stop();
	return 0;
}

page_dir_entry set_page_dir_entry(u32int present, u32int write, u32int user, u32int frame){
	page_dir_entry entry;
	entry.present = present;
	entry.rw = write;
	entry.user = user;
	entry.writet = 0;
	entry.cache = 0;
	entry.accessed = 0;
	entry.zero = 0;
	entry.size = 0;
	entry.ignored = 0;
	entry.avail = 0;
	entry.frame = frame;
	return entry;
}

page set_page_table_entry(u32int present, u32int write, u32int user, u32int frame){
	page pg;
	pg.present = present;
	pg.rw = write;
	pg.user = user;
	pg.writet = 0;
	pg.cache = 0;
	pg.accessed = 0;
	pg.dirty = 0;
	pg.zero = 0;
	pg.global = 0;
	pg.avail = 0;
	pg.frame = frame;
	return pg;
}

/* Maps a given virtual address to a given physical address */
/* Both must be the complete address not the frame */
void map(u32int va, u32int pa, u32int flags){
	int i;
	u32int tableaddr;
	page_table* table;
	u32int dir_idx = PAGE_DIR_IDX(va);
	u32int page_idx = PAGE_TABLE_IDX(va);
	// Find the appropriate page table for virtual address.
	if(page_dir->tables[dir_idx].present == 0){
		// The page table holding this page has not been created yet.
		tableaddr = pmm_alloc_page();
		table = (page_table*)tableaddr;
		for(i = 0; i < 1024; i++){
	    	table->pages[i] = set_page_table_entry(0, 0, 0, 0);
		}
		page_dir->tables[dir_idx] = set_page_dir_entry(1, 1, 0, FRAME(tableaddr));
	}
	// Now that the page table definately exists...
	table = (page_table*)((page_dir->tables[dir_idx].frame)*PAGE_SIZE);
	table->pages[page_idx] = set_page_table_entry(flags&0x1, flags&0x2, flags&0x4, FRAME(pa));
	return;
}
