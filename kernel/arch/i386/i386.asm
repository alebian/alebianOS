; Copyright 2014, 2015 Alejandro Bezdjian
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.

GLOBAL  _lgdt
GLOBAL  _lidt
GLOBAL  _read_cr0
GLOBAL  _write_cr0
GLOBAL  _read_cr3
GLOBAL  _write_cr3
GLOBAL  _Cli
GLOBAL  _Sti
GLOBAL  _XORdx
GLOBAL  _inb
GLOBAL  _inw
GLOBAL  _indw
GLOBAL  _outb
GLOBAL  _outw
GLOBAL  _outdw
GLOBAL _cpuid

GLOBAL  _exc_14_hand
GLOBAL  _int_80h_hand
GLOBAL  _irq_00_hand
GLOBAL  _irq_01_hand
GLOBAL  _irq_12_hand

EXTERN  exc_14
EXTERN  int_80h
EXTERN  irq_00
EXTERN  irq_01
EXTERN  irq_12

SECTION .text

_read_cr0:
    mov     eax, cr0
    retn

_write_cr0:
    push    ebp
    mov     ebp, esp
    mov     eax, [ebp+8]
    mov     cr0, eax
    pop     ebp
    retn

_read_cr3:
    mov     eax, cr3
    retn

_write_cr3:
    push    ebp
    mov     ebp, esp
    mov     eax, [ebp+8]
    mov     cr3, eax
    pop     ebp
    retn

_Cli:
    cli
    ret

_Sti:
    sti
    ret

_lgdt:              ;Loads GDT
    mov eax, [esp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [eax]        ; Load the new GDT pointer

    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush   ; 0x08 is the offset to our code segment: Far jump!
.flush:
    ret

_lidt:				; Loads IDTR
    push    ebp
    mov     ebp, esp
    push    ebx
    mov     ebx, [ss: ebp + 6] ; ds:bx = IDTR pointer 
    rol	    ebx, 16		    	
    lidt    [ds: ebx]          ; loads IDTR
    pop     ebx
    pop     ebp
    retn

_inb:
    push    ebp
    mov     ebp, esp

    mov     eax, 0
    mov     dx, word[ebp + 8]
    in      al, dx

    mov     esp, ebp
    pop     ebp
    ret

_inw:
    push    ebp
    mov     ebp, esp

    mov     eax, 0
    mov     dx, word[ebp + 8]
    in      ax, dx

    mov     esp, ebp
    pop     ebp
    ret

_indw:
    push    ebp
    mov     ebp, esp

    mov     eax, 0
    mov     dx, word[ebp + 8]
    in      eax, dx

    mov     esp, ebp
    pop     ebp
    ret

_outb:
    push    ebp
    mov     ebp, esp

    mov     dx, word[ebp + 8]
    mov     al, byte[ebp + 12]
    out     dx, al

    mov     esp, ebp
    pop     ebp
    ret

_outw:
    push    ebp
    mov     ebp, esp

    mov     dx, word[ebp + 8]
    mov     ax, word[ebp + 12]
    out     dx, ax

    mov     esp, ebp
    pop     ebp
    ret

_outdw:
    push    ebp
    mov     ebp, esp

    mov     dx, word[ebp + 8]
    mov     eax, dword[ebp + 12]
    out     dx, eax

    mov     esp, ebp
    pop     ebp
    ret

_exc_14_hand:           ; Page fault handler
    pusha

    mov     eax, cr2
    push    eax
    call    exc_14
    pop     eax
    
    popa
    ret

_int_80h_hand:           ; Syscall
    push    edx
    push    ecx
    push    ebx
    push    eax
    call    int_80h
    pop     eax
    pop     ebx
    pop     ecx
    pop     edx
    ret

_irq_00_hand:           ; Timer tick handler
    pusha

    call    irq_00

    jmp     EOI

_irq_01_hand:           ; Keyboard handler
    cli
    pusha

    in      al, 060h
    push    eax
    call    irq_01
    pop     eax

    jmp     EOI

_irq_12_hand:           ; Mouse handler
    cli
    pusha

    in      al, 060h
    push    eax
    call    irq_12
    pop     eax
    
    jmp     EOI2

EOI:
    mov     al, 20h
    out     20h, al
    popa
    sti
    iret

EOI2:
    mov     al, 20h
    out     20h, al
    out     0A0h, al
    popa
    sti
    iret
