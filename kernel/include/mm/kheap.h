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

#ifndef _kheap_
#define _kheap_

#include "../../../common.h"

/**
   Allocate a chunk of memory, sz in size. If align == 1,
   the chunk must be page-aligned. If phys != 0, the physical
   location of the allocated chunk will be stored into phys.

   This is the internal version of kmalloc. More user-friendly
   parameter representations are available in kmalloc, kmalloc_a,
   kmalloc_ap, kmalloc_p.
**/
uint32_t kmalloc_int(uint32_t sz, int align, uint32_t *phys);

/**
   Allocate a chunk of memory, sz in size. The chunk must be
   page aligned.
**/
uint32_t kmalloc_a(uint32_t sz);

/**
   Allocate a chunk of memory, sz in size. The physical address
   is returned in phys. Phys MUST be a valid pointer to uint32_t!
**/
uint32_t kmalloc_p(uint32_t sz, uint32_t *phys);

/**
   Allocate a chunk of memory, sz in size. The physical address 
   is returned in phys. It must be page-aligned.
**/
uint32_t kmalloc_ap(uint32_t sz, uint32_t *phys);

/**
   General allocation function.
**/
uint32_t kmalloc(uint32_t sz);

#endif
