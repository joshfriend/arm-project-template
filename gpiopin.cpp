#include <stdint.h>

#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/rom.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include "gpiopin.h"

typedef struct {
    uint32_t base;
    uint32_t sysctl_periph;
} gpio_sysctl_map_t;

gpio_sysctl_map_t sysctl_portmap[] = {
    {GPIO_PORTA_BASE, SYSCTL_PERIPH_GPIOA},
    {GPIO_PORTB_BASE, SYSCTL_PERIPH_GPIOB},
    {GPIO_PORTC_BASE, SYSCTL_PERIPH_GPIOC},
    {GPIO_PORTD_BASE, SYSCTL_PERIPH_GPIOD},
    {GPIO_PORTE_BASE, SYSCTL_PERIPH_GPIOE},
    {GPIO_PORTF_BASE, SYSCTL_PERIPH_GPIOF},
    {GPIO_PORTG_BASE, SYSCTL_PERIPH_GPIOG},
    {GPIO_PORTH_BASE, SYSCTL_PERIPH_GPIOH},
    {GPIO_PORTJ_BASE, SYSCTL_PERIPH_GPIOJ},
    {GPIO_PORTL_BASE, SYSCTL_PERIPH_GPIOL},
    {GPIO_PORTN_BASE, SYSCTL_PERIPH_GPION},
    {GPIO_PORTQ_BASE, SYSCTL_PERIPH_GPIOQ},
};

#define NUM_GPIO_PORTS 12

GPIOPin::GPIOPin(uint32_t _port, uint32_t _pin) {
    port = _port;
    pin = _pin;
    dir = GPIO_DIR_MODE_IN;
    type = GPIO_PIN_TYPE_STD;
    drive = GPIO_STRENGTH_2MA;

    // Get SysCtl register and enable peripheral
    for(uint8_t i=0; i<NUM_GPIO_PORTS; i++) {
        if(sysctl_portmap[i].base == port) {
            ROM_SysCtlPeripheralEnable(sysctl_portmap[i].sysctl_periph);
            break;
        }
    }

    // Port default is input
    ROM_GPIOPinTypeGPIOInput(port, 1<<pin);
}

void GPIOPin::configure(uint32_t _type, uint32_t _dir, uint32_t _drive) {
    type = _type;
    dir = _dir;
    drive = _drive;
    ROM_GPIOPadConfigSet(port, 1<<pin, drive, type);
}

void GPIOPin::set_direction(uint32_t _dir) {
    dir = _dir;
    ROM_GPIODirModeSet(port, 1<<pin, dir);
}

void GPIOPin::set_type(uint32_t _type) {
    type = _type;
    ROM_GPIOPadConfigSet(port, 1<<pin, drive, type);
}

void GPIOPin::set_drive_strength(uint32_t _drive) {
    drive = _drive;
    ROM_GPIOPadConfigSet(port, 1<<pin, drive, type);
}

void GPIOPin::write(uint32_t x) {
    ROM_GPIOPinWrite(port, 1<<pin, (x != 0) ? (1<<pin) : 0);
}

void GPIOPin::toggle(void) {
    write(!read());
}

uint32_t GPIOPin::read(void) {
    return (ROM_GPIOPinRead(port, 1<<pin) != 0);
}
