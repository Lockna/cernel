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
	/// write character to port 0xe9
	asm volatile("outb %0, $0xe9" :: "a"(c));
}
