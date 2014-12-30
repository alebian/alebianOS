GLOBAL  _read_msw
GLOBAL  _lidt
GLOBAL  _int_08_hand
GLOBAL  _int_09_hand
GLOBAL  _maskPIC1
GLOBAL  _maskPIC2
GLOBAL  _read_cr0
GLOBAL  _write_cr0
GLOBAL  _read_cr3
GLOBAL  _write_cr3
GLOBAL  _Cli
GLOBAL  _Sti
GLOBAL _inb
GLOBAL _inb32
GLOBAL _outb
GLOBAL _outb32

GLOBAL  _debug

EXTERN  int_08
EXTERN  int_09

SECTION .text

_read_cr0:
    mov eax, cr0
    retn

_write_cr0:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr0,  eax
    pop ebp
    retn

_read_cr3:
    mov eax, cr3
    retn

_write_cr3:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3, eax
    pop ebp
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
    out	    21h,al
    pop     ebp
    retn

_maskPIC2:			; Writes second PIC mask
    push    ebp
    mov     ebp, esp
    mov     ax, [ss:ebp+8]
    out	0A1h,al
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
    rol	ebx,16		    	
    lidt    [ds: ebx]          ; loads IDTR
    pop     ebx
    pop     ebp
    retn

_int_08_hand:			; Timer tick handler
    pusha              
    call    int_08           
    jmp EOI

_int_09_hand:
    cli
    pusha

    in al,060h
    push eax
    call int_09
    pop eax

    jmp EOI

EOI:
    mov     al,20h
    out     20h,al
    popa
    sti
    iret

_inb:
    push ebp
    mov ebp, esp

    mov eax, 0
    mov dx, word[ebp + 8]
    in al, dx

    mov esp,ebp
    pop ebp
    ret

_inb32:
    push ebp
    mov ebp, esp

    mov eax, 0
    mov dx, word[ebp + 8]
    in eax, dx

    mov esp,ebp
    pop ebp
    ret

_outb:
    push ebp
    mov ebp, esp

    mov dx,word[ebp + 8]
    mov al,byte[ebp + 12]
    out dx, al

    mov esp, ebp
    pop ebp
    ret

_outb32:
    push ebp
    mov ebp, esp

    mov dx,word[ebp + 8]
    mov eax,dword[ebp + 12]
    out dx, eax

    mov esp, ebp
    pop ebp
    ret

; BOCHS debug, stops execution; To continue put in BOCHSDBG: set $eax=0;
_debug:
    push    bp
    mov     bp, sp
    push	ax
back:	
    mov     ax, 1
    cmp	    ax, 0
    jne	    back
    pop	    ax
    pop     bp
    retn