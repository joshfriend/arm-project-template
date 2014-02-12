#include <stdint.h>

#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/debug.h>

#include "gpiopin.h"

static void (*port_a_cb_funcs[8])(void);
static void (*port_b_cb_funcs[8])(void);
static void (*port_c_cb_funcs[8])(void);
static void (*port_d_cb_funcs[8])(void);
static void (*port_e_cb_funcs[8])(void);
static void (*port_f_cb_funcs[8])(void);

// Need to associate GPIO port base with SysCtl registers:
typedef struct {
    uint32_t periph_base;
    uint32_t sysctl_reg;
} gpio_port_info_t;

// List of all the GPIO ports in order
gpio_port_info_t ports[] = {
    {GPIO_PORTA_BASE, SYSCTL_PERIPH_GPIOA},
    {GPIO_PORTB_BASE, SYSCTL_PERIPH_GPIOB},
    {GPIO_PORTC_BASE, SYSCTL_PERIPH_GPIOC},
    {GPIO_PORTD_BASE, SYSCTL_PERIPH_GPIOD},
    {GPIO_PORTE_BASE, SYSCTL_PERIPH_GPIOE},
    {GPIO_PORTF_BASE, SYSCTL_PERIPH_GPIOF},
};

#define NUM_GPIO_PORTS sizeof(portmap) / sizeof(*portmap)
#define NUM_PINS_PER_PORT 8

GPIOPin::GPIOPin(uint32_t _port, uint32_t _pin) {
    // Check parameters
    ASSERT(_port < NUM_GPIO_PORTS);
    ASSERT(_pin < NUM_PINS_PER_PORT);

    // Get driverlib parameters for pin/port
    port_base = ports[_port].periph_base;
    pin_mask  = 1 << _pin;

    // Save default parameters
    dir   = GPIO_DIR_MODE_IN;
    type  = GPIO_PIN_TYPE_STD;
    drive = GPIO_STRENGTH_2MA;

    // Enable peripheral
    MAP_SysCtlPeripheralEnable(ports[_port].sysctl_reg);

    // Delay at least 5 cycles to avoid bus fault
    SysCtlDelay(2);

    // Port default is input
    MAP_GPIOPinTypeGPIOInput(port_base, pin_mask);
}

void GPIOPin::configure(uint32_t _type, uint32_t _dir, uint32_t _drive) {
    type = _type;
    dir = _dir;
    drive = _drive;
    set_direction(dir);
    MAP_GPIOPadConfigSet(port_base, pin_mask, drive, type);
}

void GPIOPin::set_direction(uint32_t _dir) {
    dir = _dir;
    MAP_GPIODirModeSet(port_base, pin_mask, dir);
}

void GPIOPin::set_type(uint32_t _type) {
    type = _type;
    MAP_GPIOPadConfigSet(port_base, pin_mask, drive, type);
}

void GPIOPin::set_drive_strength(uint32_t _drive) {
    drive = _drive;
    MAP_GPIOPadConfigSet(port_base, pin_mask, drive, type);
}

void GPIOPin::write(uint32_t x) {
    MAP_GPIOPinWrite(port_base, pin_mask, (x != 0) ? pin_mask : 0);
}

void GPIOPin::toggle(void) {
    write(!read());
}

uint32_t GPIOPin::read(void) {
    return (MAP_GPIOPinRead(port_base, pin_mask) != 0);
}

void GPIOPin::attach_callback(uint32_t event, void(*callback)(void)) {
    ;
}
