#ifndef _asm_
#define _asm_

#include "defs.h"

unsigned int	_read_msw();

void	_lidt (IDTR *idtr);

void	_maskPIC1 (byte mask);
void	_maskPIC2 (byte mask);

void	_Cli(void);
void	_Sti(void);

void	_int_08_hand();
void	_int_09_hand();

/* Outport */
void _outb(word port, byte source);
void _outb32(word port, dword source);
/* Inport */
byte _inb(word port);
dword _inb32(word port);

void	_debug (void);

#endif