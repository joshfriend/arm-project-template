#include <inc/hw_types.h>
#include <inc/hw_memmap.h>
#include <driverlib/rom.h>
#include <driverlib/rom_map.h>
#include <driverlib/fpu.h>
#include <driverlib/sysctl.h>
#include <driverlib/watchdog.h>

#include "compiler.h"

__section(".init")
void clock_init(void) {
    // Run directly from the crystal
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_1   |
                       SYSCTL_USE_PLL    |
                       SYSCTL_XTAL_16MHZ |
                       SYSCTL_OSC_MAIN);
}

__section(".init")
void fpu_init(void) {
    // Enable FPU.
    // Must be done in privileged mode
    MAP_FPUEnable();

    // Delay at least 5 cycles to avoid bus fault
    SysCtlDelay(2);

    /*
      Enable FPU stacking for using floating point
      operations inside interrupt handlers.

      Lazy mode allows floating point math in interrupts,
      but with added stack usage and slower exception response time.
      Lazy mode only restores FPU state if exception occurred while
      a floating point instruction was in execution when the
      exception occurred.

      Use regular FPU stacking to save/restore the FPU state
      on all exception entry/exit.
     */

    // MAP_FPULazyStackingEnable();

    // MAP_FPUStackingEnable();
}

__section(".init")
void wdt_init(void) {
    // Enable watchdog peripheral
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

    // Delay at least 5 cycles to avoid bus fault
    SysCtlDelay(2);

    // Unlock peripheral
    MAP_WatchdogUnlock(WATCHDOG0_BASE);
    MAP_WatchdogEnable(WATCHDOG0_BASE);

    // Reset CPU on expire
    MAP_WatchdogResetEnable(WATCHDOG0_BASE);

    // Stalls the WDT during debug events
    MAP_WatchdogStallEnable(WATCHDOG0_BASE);

    // Reload period set to 1 second
    MAP_WatchdogReloadSet(WATCHDOG0_BASE, MAP_SysCtlClockGet());

    // Lock peripheral
    MAP_WatchdogLock(WATCHDOG0_BASE);
}
