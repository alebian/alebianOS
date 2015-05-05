ENTRY (start)
SECTIONS{
    . = 1M;

    .text : {
        code = .;
        *(.multiboot)
        *(.text)
    }

    .rodata : {
        rodata = .;
        *(.rodata)
    }

    .data : {
        data = .;
        *(.data)
    }

    .bss : {
        bss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }

    end = .;
}
