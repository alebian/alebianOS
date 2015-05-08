/*
* Copyright 2014 Alejandro Bezdjian
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

#ifndef _idt_
#define _idt_

#include "kernel.h"

#define IDT_SIZE 256

/* Interrupt descriptor */
typedef struct {
  word      offset_l;
  word      selector;
  byte      zero;
  byte      access;
  word      offset_h;
} __attribute__ ((packed)) INT_DESCR;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} __attribute__ ((packed)) IDTR;

int init_IDT(void);
void init_IDT_content(void);
void remapIRQ(void);
void init_IDT_entry(u8int, byte, dword, byte);
void init_IDTR(void);
void setup_PIC(void);

#endif