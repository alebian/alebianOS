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

global mboot
extern code, bss, end ; Defined in link.ld

;**************************************************************
; GRUB header
;**************************************************************;

mboot:
	dd MULTIBOOT_HEADER_MAGIC  ; Header's identity number, must be 0x1BADB002
	dd MULTIBOOT_HEADER_FLAGS  ; 0-15 bits indicate image's requirements, 16-31 bits indicate optional features
	dd MULTIBOOT_CHECKSUM

	dd mboot	; Header start point.
	dd code		; Text segment start point.
	dd bss		; Segment final point. ( a.out format)
	dd end
	dd start	; Img entry point.

global start		; making entry point visible to linker
extern kmain		; _main is defined elsewhere

[global start]
[extern kmain] ; in main.c

start:
	mov esp, stack + 32768
	push eax		; pass Multiboot magic number
	push ebx		; pass Multiboot info structure
	mov ebp, 0
  	
  	call kmain
  	jmp $

section .bss
stack:
    ;resb 32768