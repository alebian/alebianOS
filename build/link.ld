ENTRY (start)
SECTIONS{
    . = 1M;

    .text : {
        code = .;
        *(.multiboot)
        *(.text)
        . = ALIGN(4096);
    }

    .rodata : {
        rodata = .;
        *(.rodata)
        . = ALIGN(4096);
    }

    .data : {
        data = .;
        *(.data)
        . = ALIGN(4096);
    }

    .bss : {
        bss = .;
        *(COMMON)
        *(.bss)
        . = ALIGN(4096);
    }

    end = .;
}
