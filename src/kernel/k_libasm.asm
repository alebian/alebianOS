GLOBAL  _read_msw
GLOBAL  _lidt
GLOBAL  _maskPIC1
GLOBAL  _maskPIC2
GLOBAL  _read_cr0
GLOBAL  _write_cr0
GLOBAL  _read_cr3
GLOBAL  _write_cr3
GLOBAL  _Cli
GLOBAL  _Sti
GLOBAL  _XORdx
GLOBAL  _inb
GLOBAL  _indw
GLOBAL  _outb
GLOBAL  _outdw
GLOBAL _turnOff

GLOBAL  _int_14_hand
GLOBAL  _irq_00_hand
GLOBAL  _irq_01_hand
GLOBAL  _irq_12_hand

EXTERN  int_14
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

_maskPIC1:			; Writes first PIC mask
    push    ebp
    mov     ebp, esp
    mov     ax, [ss:ebp+8]
    out	    21h, al
    pop     ebp
    retn

_maskPIC2:			; Writes second PIC mask
    push    ebp
    mov     ebp, esp
    mov     ax, [ss:ebp+8]
    out	    0A1h, al
    pop     ebp
    retn

_read_msw:
    smsw    ax		; Gets Machine Status Word
    retn

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

_outdw:
    push    ebp
    mov     ebp, esp

    mov     dx, word[ebp + 8]
    mov     eax, dword[ebp + 12]
    out     dx, eax

    mov     esp, ebp
    pop     ebp
    ret

_int_14_hand:           ; Page fault handler
    pusha

    mov     eax, cr2
    push    eax
    call    int_14
    pop     eax
    
    popa
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
