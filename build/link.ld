ENTRY (_loader)
SECTIONS{
    .text 0x00100000 :{
        code = .;
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
        _sbss = .;
        *(COMMON)
        *(.bss)
        _ebss = .;
    }

    end = .;
}
