#include <stdint.h>

#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/debug.h>
#include <driverlib/interrupt.h>

#include "gpiopin.h"
#include "compiler.h"

// Need to associate GPIO port base with SysCtl registers:
typedef struct {
    uint32_t base;
    uint32_t sysctl_reg;
    uint32_t int_num;
} gpio_port_info_t;

// List of all the GPIO ports in order
static const gpio_port_info_t ports[] = {
    {GPIO_PORTA_BASE, SYSCTL_PERIPH_GPIOA, INT_GPIOA},
    {GPIO_PORTB_BASE, SYSCTL_PERIPH_GPIOB, INT_GPIOB},
    {GPIO_PORTC_BASE, SYSCTL_PERIPH_GPIOC, INT_GPIOC},
    {GPIO_PORTD_BASE, SYSCTL_PERIPH_GPIOD, INT_GPIOD},
    {GPIO_PORTE_BASE, SYSCTL_PERIPH_GPIOE, INT_GPIOE},
    {GPIO_PORTF_BASE, SYSCTL_PERIPH_GPIOF, INT_GPIOF},
};

#define NUM_GPIO_PORTS sizeof(ports) / sizeof(*ports)
#define NUM_PINS_PER_PORT 8

// Pin interrupt callbacks
static gpio_pin_int_cb_t gpio_pin_callbacks[NUM_GPIO_PORTS][NUM_PINS_PER_PORT];


// Private function prototypes
static void attach_exception_handlers(void);
static void gpio_port_a_exception_handler(void);
static void gpio_port_b_exception_handler(void);
static void gpio_port_c_exception_handler(void);
static void gpio_port_d_exception_handler(void);
static void gpio_port_e_exception_handler(void);
static void gpio_port_f_exception_handler(void);
static void gpio_master_exception_handler(uint32_t port_num);


GPIOPin::GPIOPin(uint32_t _port, uint32_t _pin) {
    // Check parameters
    ASSERT(_port < NUM_GPIO_PORTS);
    ASSERT(_pin < NUM_PINS_PER_PORT);

    port_num = _port;
    pin_num  = _pin;

    // Get driverlib parameters for pin/port
    port_base = ports[_port].base;
    pin_mask  = 1 << _pin;

    // Save default parameters
    config.dir   = GPIO_PIN_DIR_IN;
    config.mode  = GPIO_PIN_MODE_STD;
    config.drive = GPIO_PIN_DRIVE_2MA;

    // Enable peripheral
    MAP_SysCtlPeripheralEnable(ports[_port].sysctl_reg);

    // Delay at least 5 cycles to avoid bus fault
    SysCtlDelay(2);

    // Port default is input
    MAP_GPIOPinTypeGPIOInput(port_base, pin_mask);
}

void GPIOPin::operator=(uint32_t x)
{
    write(x);
}

void GPIOPin::configure(gpio_pin_cfg_t *cfg) {
    uint32_t d = 0;
    uint32_t m = 0;

    // Check parameters
    ASSERT(cfg->dir < GPIO_PIN_DIR_TOTAL);
    ASSERT(cfg->mode < GPIO_PIN_MODE_TOTAL);
    ASSERT(cfg->drive < GPIO_PIN_DRIVE_TOTAL);

    // Save settings
    config.dir   = cfg->dir;
    config.mode  = cfg->mode;
    config.drive = cfg->drive;

    switch(cfg->drive) {
        case GPIO_PIN_DRIVE_2MA:
            d = GPIO_STRENGTH_2MA;
            break;
        case GPIO_PIN_DRIVE_4MA:
            d = GPIO_STRENGTH_4MA;
            break;
        case GPIO_PIN_DRIVE_8MA:
            d = GPIO_STRENGTH_8MA;
            break;
        case GPIO_PIN_DRIVE_8MA_SC:
            d = GPIO_STRENGTH_8MA_SC;
            break;
        default:
            break;
    }

    switch(cfg->mode) {
        case GPIO_PIN_MODE_STD:
            m = GPIO_PIN_TYPE_STD;
            break;
        case GPIO_PIN_MODE_STD_WPU:
            m = GPIO_PIN_TYPE_STD_WPU;
            break;
        case GPIO_PIN_MODE_STD_WPD:
            m = GPIO_PIN_TYPE_STD_WPD;
            break;
        case GPIO_PIN_MODE_OD:
            m = GPIO_PIN_TYPE_OD;
            break;
        case GPIO_PIN_MODE_OD_WPU:
            m = GPIO_PIN_TYPE_OD_WPU;
            break;
        case GPIO_PIN_MODE_OD_WPD:
            m = GPIO_PIN_TYPE_OD_WPD;
            break;
        case GPIO_PIN_MODE_ANALOG:
            m = GPIO_PIN_TYPE_ANALOG;
            break;
        default:
            break;
    }

    // Apply settings
    set_direction(config.dir);
    MAP_GPIOPadConfigSet(port_base, pin_mask, d, m);
}

void GPIOPin::set_direction(gpio_pin_dir_t dir) {
    // Check parameters
    ASSERT(dir < GPIO_PIN_DIR_TOTAL);

    // Save settings
    config.dir = dir;

    // Apply settings
    configure(&config);
}

void GPIOPin::set_mode(gpio_pin_mode_t mode) {
    // Check parameters
    ASSERT(mode < GPIO_PIN_MODE_TOTAL);

    // Save settings
    config.mode = mode;

    // Apply settings
    configure(&config);
}

void GPIOPin::set_drive_strength(gpio_pin_drive_t drive) {
    // Check parameters
    ASSERT(drive < GPIO_PIN_DRIVE_TOTAL);

    // Save settings
    config.drive = drive;

    // Apply settings
    configure(&config);

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

void GPIOPin::attach_callback(gpio_pin_int_type_t event, void(*callback)(void)) {
    // Check parameters
    ASSERT(event < GPIO_PIN_INT_TOTAL);

    uint32_t int_type = 0;

    switch(event) {
        case GPIO_PIN_INT_LOW:
            int_type = GPIO_FALLING_EDGE;
            break;
        case GPIO_PIN_INT_HIGH:
            int_type = GPIO_RISING_EDGE;
            break;
        case GPIO_PIN_INT_RISING:
            int_type = GPIO_BOTH_EDGES;
            break;
        case GPIO_PIN_INT_FALLING:
            int_type = GPIO_LOW_LEVEL;
            break;
        case GPIO_PIN_INT_BOTH:
            int_type = GPIO_HIGH_LEVEL;
            break;
        default:
            // type NONE or invalid
            return;
    }

    gpio_pin_callbacks[port_num][pin_num] = callback;

    // Apply settings
    MAP_GPIOPinIntClear(port_base, pin_mask);
    MAP_GPIOPinIntEnable(port_base, pin_mask);
    MAP_GPIOIntTypeSet(port_base, pin_mask, int_type);
    MAP_IntEnable(ports[port_num].int_num);
    IntMasterEnable();
}

void GPIOPin::detach_callback(void) {
    uint32_t i;

    // Erase callback
    for(i=0; i < NUM_GPIO_PORTS; i++) {
        if(ports[i].base == port_base) {
            gpio_pin_callbacks[i][pin_num] = 0;
        }
    }

    // Apply settings
    MAP_GPIOPinIntDisable(port_base, pin_mask);
    MAP_GPIOIntTypeSet(port_base, pin_mask, GPIO_PIN_INT_NONE);
}

#pragma GCC diagnostic ignored "-Wunused-function"
__section(".init")
static void attach_exception_handlers(void) {
    IntRegister(INT_GPIOA, gpio_port_a_exception_handler);
    IntRegister(INT_GPIOB, gpio_port_b_exception_handler);
    IntRegister(INT_GPIOC, gpio_port_c_exception_handler);
    IntRegister(INT_GPIOD, gpio_port_d_exception_handler);
    IntRegister(INT_GPIOE, gpio_port_e_exception_handler);
    IntRegister(INT_GPIOF, gpio_port_f_exception_handler);
}

static void gpio_port_a_exception_handler(void) {
    gpio_master_exception_handler(0);
}

static void gpio_port_b_exception_handler(void) {
    gpio_master_exception_handler(1);
}

static void gpio_port_c_exception_handler(void) {
    gpio_master_exception_handler(2);
}

static void gpio_port_d_exception_handler(void) {
    gpio_master_exception_handler(3);
}

static void gpio_port_e_exception_handler(void) {
    gpio_master_exception_handler(4);
}

static void gpio_port_f_exception_handler(void) {
    gpio_master_exception_handler(5);
}

static void gpio_master_exception_handler(uint32_t port_num) {
    uint32_t i;
    uint32_t isr = GPIOPinIntStatus(ports[port_num].base, true);

    MAP_GPIOPinIntClear(ports[port_num].base, isr);

    // Execute callbacks
    for(i=0; i<NUM_GPIO_PORTS; i++) {
        if(isr & (1 << i)) {
            if(gpio_pin_callbacks[port_num][i]) {
                gpio_pin_callbacks[port_num][i]();
            }
        }
    }
}
