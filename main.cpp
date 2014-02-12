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
#include <stdbool.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include <driverlib/rom_map.h>
#include <driverlib/sysctl.h>

#include "gpiopin.h"

#ifdef __cplusplus
extern "C" {
#endif

int main(void) {
    // Configure GPIO
    GPIOPin blue_led = GPIOPin(5, 2);
    blue_led.set_direction(GPIO_PIN_DIR_OUT);

    while(1)
    {
        // Blink the BLUE LED
        MAP_SysCtlDelay(MAP_SysCtlClockGet() / 10 / 3);
        blue_led.toggle();
    }
}

#ifdef __cplusplus
}
#endif
