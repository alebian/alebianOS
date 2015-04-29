ENTRY (_loader)
SECTIONS{
    .text 0x00100000 :{
        code = .;
        *(.text)
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
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
        . = ALIGN(4096);
    }

    end = .;
}
