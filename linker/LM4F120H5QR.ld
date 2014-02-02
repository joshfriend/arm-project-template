MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.nvic_table))
        *(.text*)
        *(.rodata*)
    } > FLASH

    .ARM.exidx :
    {
        *(.ARM.exidx)
        _etext = .;
    } > FLASH

    .data : AT(ADDR(.ARM.exidx) + SIZEOF(.ARM.exidx))
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
        end = .; /* sbrk() syscall expects this */
    } > SRAM
}
