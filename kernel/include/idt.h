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

#include "system.h"
#include "../../common.h"

#define IDT_SIZE 256

/* Interrupt descriptor */
typedef struct {
  sint16_t      offset_l;
  sint16_t      selector;
  uint8_t      zero;
  uint8_t      access;
  sint16_t      offset_h;
} __attribute__ ((packed)) INT_DESCR;

/* IDTR  */
typedef struct {
  sint16_t  limit;
  sint32_t base;
} __attribute__ ((packed)) IDTR;

int init_IDT(void);
void init_IDT_content(void);
void remapIRQ(void);
void init_IDT_entry(uint8_t, uint8_t, sint32_t, uint8_t);
void init_IDTR(void);
void setup_PIC(void);

#endif