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

#include <stdint.h>
#include <inc/hw_types.h>
#include <inc/hw_nvic.h>
#include <driverlib/rom.h>

#include "compiler.h"

// Application entry point
extern int main(void);

// Default exception handlers
void reset_handler(void);
void nmi_handler(void);
void hardfault_handler(void);
void default_handler(void);

// Linker defined sections
extern uint32_t _etext;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss;
extern uint32_t _ebss;
extern uint32_t _stack_top;
extern void (*__init_array_start[])(void);
extern void (*__init_array_end[])(void);
extern void (*__fini_array_start[])(void);
extern void (*__fini_array_end[])(void);

// Typedef for exception handler function
typedef void (*nvic_handler_t)(void);

// NVIC exception table
__section(".nvic_table")
nvic_handler_t nvic_table[] = {
    // Stack pointer starts at top of RAM
    // Stack size is defined by linker script
    (nvic_handler_t)&_stack_top,

    // Fixed priority exceptions
    reset_handler,          // code entry point                 1
    nmi_handler,            // NMI handler.                     2
    hardfault_handler,      // hard fault handler.              3

    // Configurable priority exceptions.
    default_handler,        // Memory Management Fault          4
    default_handler,        // Bus Fault                        5
    default_handler,        // Usage Fault                      6
    0,                      // Reserved                         7
    0,                      // Reserved                         8
    0,                      // Reserved                         9
    0,                      // Reserved                         10
    default_handler,        // SV call                          11
    default_handler,        // Debug monitor                    12
    0,                      // Reserved                         13
    default_handler,        // PendSV                           14
    default_handler,        // SysTick                          15

    // Peripherial exceptions start here.
    default_handler,        // GPIO Port A                      16
    default_handler,        // GPIO Port B                      17
    default_handler,        // GPIO Port C                      18
    default_handler,        // GPIO Port D                      19
    default_handler,        // GPIO Port E                      20
    default_handler,        // UART 0                           21
    default_handler,        // UART 1                           22
    default_handler,        // SSI 0                            23
    default_handler,        // I2C 0                            24
    0,                      // Reserved                         25
    0,                      // Reserved                         26
    0,                      // Reserved                         27
    0,                      // Reserved                         28
    0,                      // Reserved                         29
    default_handler,        // ADC 0 Seq 0                      30
    default_handler,        // ADC 0 Seq 1                      31
    default_handler,        // ADC 0 Seq 2                      32
    default_handler,        // ADC 0 Seq 3                      33
    default_handler,        // WDT 0 and 1                      34
    default_handler,        // 16/32 bit timer 0 A              35
    default_handler,        // 16/32 bit timer 0 B              36
    default_handler,        // 16/32 bit timer 1 A              37
    default_handler,        // 16/32 bit timer 1 B              38
    default_handler,        // 16/32 bit timer 2 A              39
    default_handler,        // 16/32 bit timer 2 B              40
    default_handler,        // Analog comparator 0              41
    default_handler,        // Analog comparator 1              42
    0,                      // Reserved                         43
    default_handler,        // System control                   44
    default_handler,        // Flash + EEPROM control           45
    default_handler,        // GPIO Port F                      46
    0,                      // Reserved                         47
    0,                      // Reserved                         48
    default_handler,        // UART 2                           49
    default_handler,        // SSI 1                            50
    default_handler,        // 16/32 bit timer 3 A              51
    default_handler,        // 16/32 bit timer 3 B              52
    default_handler,        // I2C 1                            53
    0,                      // Reserved                         54
    default_handler,        // CAN 0                            55
    0,                      // Reserved                         56
    0,                      // Reserved                         57
    0,                      // Reserved                         58
    default_handler,        // Hibernation module               59
    default_handler,        // USB                              60
    0,                      // Reserved                         61
    default_handler,        // UDMA SW                          62
    default_handler,        // UDMA Error                       63
    default_handler,        // ADC 1 Seq 0                      64
    default_handler,        // ADC 1 Seq 1                      65
    default_handler,        // ADC 1 Seq 2                      66
    default_handler,        // ADC 1 Seq 3                      67
    0,                      // Reserved                         68
    0,                      // Reserved                         69
    0,                      // Reserved                         70
    0,                      // Reserved                         71
    0,                      // Reserved                         72
    default_handler,        // SSI 2                            73
    default_handler,        // SSI 2                            74
    default_handler,        // UART 3                           75
    default_handler,        // UART 4                           76
    default_handler,        // UART 5                           77
    default_handler,        // UART 6                           78
    default_handler,        // UART 7                           79
    0,                      // Reserved                         80
    0,                      // Reserved                         81
    0,                      // Reserved                         82
    0,                      // Reserved                         83
    default_handler,        // I2C 2                            84
    default_handler,        // I2C 4                            85
    default_handler,        // 16/32 bit timer 4 A              86
    default_handler,        // 16/32 bit timer 4 B              87
    0,                      // Reserved                         88
    0,                      // Reserved                         89
    0,                      // Reserved                         90
    0,                      // Reserved                         91
    0,                      // Reserved                         92
    0,                      // Reserved                         93
    0,                      // Reserved                         94
    0,                      // Reserved                         95
    0,                      // Reserved                         96
    0,                      // Reserved                         97
    0,                      // Reserved                         98
    0,                      // Reserved                         99
    0,                      // Reserved                         100
    0,                      // Reserved                         101
    0,                      // Reserved                         102
    0,                      // Reserved                         103
    0,                      // Reserved                         104
    0,                      // Reserved                         105
    0,                      // Reserved                         106
    0,                      // Reserved                         107
    default_handler,        // 16/32 bit timer 5 A              108
    default_handler,        // 16/32 bit timer 5 B              109
    default_handler,        // 32/64 bit timer 0 A              110
    default_handler,        // 32/64 bit timer 0 B              111
    default_handler,        // 32/64 bit timer 1 A              112
    default_handler,        // 32/64 bit timer 1 B              113
    default_handler,        // 32/64 bit timer 2 A              114
    default_handler,        // 32/64 bit timer 2 B              115
    default_handler,        // 32/64 bit timer 3 A              116
    default_handler,        // 32/64 bit timer 3 B              117
    default_handler,        // 32/64 bit timer 4 A              118
    default_handler,        // 32/64 bit timer 4 B              119
    default_handler,        // 32/64 bit timer 5 A              120
    default_handler,        // 32/64 bit timer 5 B              121
    default_handler,        // System Exception                 122
    0,                      // Reserved                         123
    0,                      // Reserved                         124
    0,                      // Reserved                         125
    0,                      // Reserved                         126
    0,                      // Reserved                         127
    0,                      // Reserved                         128
    0,                      // Reserved                         129
    0,                      // Reserved                         130
    0,                      // Reserved                         131
    0,                      // Reserved                         132
    0,                      // Reserved                         133
    0,                      // Reserved                         134
    0,                      // Reserved                         135
    0,                      // Reserved                         136
    0,                      // Reserved                         137
    0,                      // Reserved                         138
    0,                      // Reserved                         139
    0,                      // Reserved                         140
    0,                      // Reserved                         141
    0,                      // Reserved                         142
    0,                      // Reserved                         143
    0,                      // Reserved                         144
    0,                      // Reserved                         145
    0,                      // Reserved                         146
    0,                      // Reserved                         147
    0,                      // Reserved                         148
    0,                      // Reserved                         149
    0,                      // Reserved                         150
    0,                      // Reserved                         151
    0,                      // Reserved                         152
    0,                      // Reserved                         153
    0                       // Reserved                         154
};

// Reset handler. Sets up for application on reset
void reset_handler(void) {
    uint32_t *src, *dest;
    uint32_t i, cnt;

    // Copy data initializers from flash to RAM
    src = &_etext;
    dest = &_data;
    while(dest < &_edata) {
        *dest++ = *src++;
    }

    // Zero fill bss
    dest = &_bss;
    while(dest < &_ebss) {
        *dest++ = 0;
    }

    // Call any initializer functions/constructors
    cnt = __init_array_end - __init_array_start;
    for(i = 0; i < cnt; i++) {
        __init_array_start[i]();
    }

    // Application entry
    main();

    // Call any finalizer functions and destructors
    cnt = __fini_array_end - __fini_array_start;
    for(i = 0; i < cnt; i++) {
        __fini_array_start[i]();
    }
}

// Non-maskable interrupt handler
void nmi_handler(void) {
    while(1);
}

// Hardfault exception handler
void hardfault_handler(void) {
    while(1);
}

// Default handler for all other exceptions
void default_handler(void) {
    while(1);
}
