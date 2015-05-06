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