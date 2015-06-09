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

#ifndef _pmm_
#define _pmm_

#include "../multiboot.h"
#include "../../../common/common.h"

int init_pmm(multiboot_info_t*);
uint32_t pmm_alloc_page(void);
void pmm_free_page(uint32_t);
void print_memory_map(multiboot_info_t*);
void pmm_paging_stop(void);

#endif