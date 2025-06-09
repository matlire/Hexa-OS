ENTRY(_start)

SECTIONS
{
    . = 1M;

    .boot BLOCK(4K) : ALIGN(4K)
    {
        KEEP(*(.multiboot_header))
    }

    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.text)
    } :text = 0x90 /* Associate .text section with PHDRS text section, fill padding with 0x90 - NOP */

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(.bss)
    }
}

/* Program Headers */
PHDRS
{
    text PT_LOAD FLAGS(0x5); /* RX */
    data PT_LOAD FLAGS(0x6); /* RW */
}