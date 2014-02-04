/*
 * Copyright (c) 2014 Josh Friend
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 512K
    SRAM  (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

/* The stack size should be the same size as SRAM */
stack_size = 32K;

SECTIONS
{
    /* Executable code stored in text */
    .text :
    {
        _text = .;

        /* Vector table is stored at the start of FLASH.
         * Name must match what is used in startup_gcc.c
         */
        KEEP(*(.nvic_table))

        *(.text*)
        *(.rodata*)
    } > FLASH

    /* Used for C++ exception handling.
     * This section is sorted and must be kept separate.
     */
    .ARM.exidx :
    {
        *(.ARM.exidx)
        _etext = .;
    } > FLASH

    /* .data stores initialiezd data */
    .data : AT(ADDR(.ARM.exidx) + SIZEOF(.ARM.exidx))
    {
        _data = .;

        /* Vector table is stored in RAM after startup.
         * Name must match what is used by TI libraries.
         */
        *(vtable)

        *(.data*)
        _edata = .;
    } > SRAM

    /* .bss stores uninitialized data */
    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;

        /* sbrk() syscall expects this */
        end = .;

        /* Create a symbol for the top of the stack */
        _stack_top = ADDR(.data) + stack_size;
    } > SRAM
}
