;*****************************************************
; start.asm
;
; Img entry point.
; Multiboot header.
;******************************************************

%include "../../../include/grub.inc"

[BITS 32]

[global start]
[extern kmain] ; in main.c

start:

  call kmain

  jmp $


;**************************************************************
; GRUB header
;**************************************************************

; Defined in link.ld
EXTERN code, bss, end

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