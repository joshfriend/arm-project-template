
#ifndef __COMPILER_H__
#define __COMPILER_H__

// Function attributes
#define __always_inline __attribute__((always_inline))
#define __naked         __attribute__((naked))
#define __signal        __attribute__((signal))

// Function/data attributes
#define __section(x)    __attribute__((section(x)))

#endif
