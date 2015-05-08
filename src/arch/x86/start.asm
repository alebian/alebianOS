;         Based on code from Bran's kernel development tutorials.
;*****************************************************
; start.asm
;
; Img entry point.
; Multiboot header.
;******************************************************

MULTIBOOT_PAGE_ALIGN	equ 1<<0
MULTIBOOT_MEMORY_INFO	equ 1<<1        
MULTIBOOT_AOUT_KLUDGE	equ 1<<16
MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

[BITS 32]

section .text

GLOBAL mboot
EXTERN code, bss, end ; Defined in link.ld

;**************************************************************
; GRUB header
;**************************************************************;
ALIGN 4
mboot:
	dd MULTIBOOT_HEADER_MAGIC  ; Header's identity number, must be 0x1BADB002
	dd MULTIBOOT_HEADER_FLAGS  ; 0-15 bits indicate image's requirements, 16-31 bits indicate optional features
	dd MULTIBOOT_CHECKSUM

	dd mboot	; Header start point.
	dd code		; Text segment start point.
	dd bss		; Segment final point. ( a.out format)
	dd end
	dd start	; Img entry point.

GLOBAL start		; making entry point visible to linker
EXTERN kmain		; _main is defined elsewhere

[GLOBAL start]
[EXTERN kmain] ; in main.c

start:
    mov  esp, stack_end  ; Set the stack pointer
	push eax		; pass Multiboot magic number
	push ebx		; pass Multiboot info structure
	mov ebp, 0
  	
  	call kmain
  	jmp $

section .bss
    resb 8192  ; Reserve stack space
stack_end: