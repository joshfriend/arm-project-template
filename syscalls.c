
#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <errno.h>
#include <reent.h>
#include <unistd.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/uart.h>

// int _system(const char *);
// int _rename(const char *, const char *);
// int _isatty(int);
// clock_t _times(struct tms *);
// int _gettimeofday(struct timeval *, struct timezone *);
// void _raise(void);
// int _unlink(void);
// int _link(void);
// int _stat(const char *, struct stat *);
// int _fstat(int, struct stat *);
// caddr_t _sbrk(int);
// int _getpid(int);
// int _kill(int, int);
// void _exit(int);
// int _close(int);
// int _open(const char *, int, ...);
// int _write(int, char *, int);
// int _lseek(int, int, int);
// int _read(int, char *, int);

// Register name faking - works in collusion with the linker.
register char * stack_ptr asm("sp");

extern char _end;

/* Return number of characters read, no more than 'len' */
int _read(int file, char *ptr, int len)
{
    long c;
    int bytesRead = 0;

    (void) file; // Indicate variable is unused

    do {
        c = UARTCharGetNonBlocking(UART1_BASE);
        if (c == (long)(-1)) {
            break;
        }
        else {
            *ptr++ = (char)(c & 0xFF);
        }
    } while (++bytesRead < len);

    return bytesRead;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

/* Return number of characters written */
int _write(int file, char *ptr, int len)
{
    unsigned i;

    for (i=0; i < len; i++) {
        UARTCharPut(UART1_BASE, *ptr++);
    }
    return len;
}

int _open(const char *path, int flags, ...)
{
    return 0;
}

int _close(int file)
{
    return 0;
}

void _exit(int n)
{
    (void) n;  // Parameter not used
    while (1);
}

int _kill(int n, int m)
{
    (void) n;
    (void) m;
    return 0;
}

int _getpid(int n)
{
    (void) n;
    return 1;
}

caddr_t _sbrk(unsigned int incr)
{
    static char * heap_end = NULL;
    char * prev_heap_end;

    if (heap_end == NULL) {
        heap_end = &_end;
    }

    prev_heap_end = heap_end;

    if (heap_end + incr > (caddr_t)stack_ptr) {
        errno = ENOMEM;
        return (caddr_t) -1;
    }

    heap_end += incr;

    return (caddr_t)prev_heap_end;
}

int _fstat(int file, struct stat * st)
{
    return 0;
}

int _stat(const char *fname, struct stat *st)
{
    return -1;
}

int _link(void)
{
    return -1;
}

int _unlink(void)
{
    return -1;
}

void _raise (void)
{
    return;
}

int _gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    return 0;
}

clock_t _times(struct tms * tp)
{
    return 0;
}

int _isatty(int fd)
{
    return 1;
    fd = fd;
}

int _system(const char *s)
{
    if (s == NULL)
        return 0;
    errno = ENOSYS;
    return -1;
}

int _rename(const char * oldpath, const char * newpath)
{
    errno = ENOSYS;
    return -1;
}
