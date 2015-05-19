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

#include "../kernel.h"

#define HEAP_START 0xD0000000
#define HEAP_END   0xFFBFF000

typedef struct header{
  struct header *prev, *next;
  u32int allocated : 1;
  u32int length : 31;
} header_t;

void init_heap(void);
// Returns a pointer to a chunk of memory, minimum size 'l' bytes.
void *kmalloc(u32int);
// Takes a chunk of memory allocated with kmalloc, and returns it to the pool of usable memory.
//void kfree(void*);
void alloc_chunk(u32int, u32int);
//void free_chunk(header_t*);
void split_chunk(header_t*, u32int);
//void glue_chunk(header_t*);

#endif
