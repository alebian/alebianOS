/*
* Copyright 2014, 2015 Alejandro Bezdjian
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
#ifndef _asm_
#define _asm_

#include "../../kernel/gdt.h"
#include "../../kernel/idt.h"

unsigned int	_read_msw(void);

void	_lgdt (GDT_PTR*);
void	_lidt (IDTR*);

/* PIC masks handlers */
void	_maskPIC1 (byte);
void	_maskPIC2 (byte);

unsigned long	_read_cr0(void);
void			_write_cr0(unsigned int);
unsigned long	_read_cr3(void);
void			_write_cr3(unsigned int);

void	_Cli(void);
void	_Sti(void);

/* Interrupt handlers */
void	_int_14_hand(void);
void	_int_80_hand(void);
void	_irq_00_hand(void);
void	_irq_01_hand(void);
void	_irq_12_hand(void);

/* Outport */
void 	_outb(word, byte);
void 	_outw(word, word);
void 	_outdw(word, dword);
/* Inport */
byte 	_inb(word);
word 	_inw(word);
dword 	_indw(word);

#endif