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
		page_dir_entry entry;
		entry.present = 1;
		entry.rw = 1;
		if(i < KERNEL_MAX_FRAMES){
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

	/* We will use Identity paging for the first 4MB */
	tableaddr = pmm_alloc_page();
	page_table* table = (page_table*)tableaddr;
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
	    table->pages[i] = pg;
	}
	page_dir->tables[0].present = 1;
	page_dir->tables[0].frame = (unsigned int)tableaddr/PAGE_SIZE;

	/* Finally we enable paging */
	_write_cr3((unsigned int)page_dir);
	_write_cr0(_read_cr0() | 0x80000000); // Set the paging bit in CR0 to 1
	/* Tell the PMM that we don't need any more pages */
	pmm_paging_stop();
	return 0;
}
