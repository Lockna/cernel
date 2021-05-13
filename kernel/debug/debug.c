/**
 *	@file debug.c
 *	@brief Debug driver
 *
 *	These functions are used to communicate with the host console.
 *	The bytes are written to I/O Port 0xe9, qemu sees the written bytes
 *	and prints them to the host console.
 */

#include <debug/debug.h>
#include <cernel/lib/print.h>
#include <stdarg.h>

void dbg_printf(char *format, ...)
{
     va_list args;
     va_start(args, format);

     intern_printf(format, args, dbgc);

     va_end(args);
}

void dbgln(char *str)
{
	while (*str != '\0') {
		dbgc(*str);
		str++;
	}
}

void dbgc(char c)
{
	/// Write byte to I/O Port 0xe9
	asm volatile("outb %0, $0xe9" :: "a"(c));
}