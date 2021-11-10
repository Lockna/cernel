/**
 *	@file debug.c
 *	@brief Debug driver
 *
 *	These functions are used to communicate with the host console.
 *	The bytes are written to I/O Port 0xe9, qemu sees the written bytes
 *	and prints them to the host console.
 */

#include <debug/debug.h>
#include <debug/symbols.h>
#include <cernel/lib/print.h>
#include <stdarg.h>

void print_stacktrace(uint64_t rip_value, uint64_t rbp_value) {

#ifndef debug
	kprintf("Stack trace not available for release builds\n");
	return;
#endif

	kprintf("======= Starting Backtrace =======\n");

	uint64_t *rbp = (uint64_t*)rbp_value;

	uint64_t address = rip_value;
	uint64_t offset = 0;
	const char *symbol_name = get_symbol_name(address, &offset);

	kprintf("    %p <%s+%u>\n", address, symbol_name, offset);

	while (*rbp != 0) {
		// print the saved rip
		address = *(rbp + 1);
		symbol_name = get_symbol_name(address, &offset);

		kprintf("    %p <%s+%u>\n", address, symbol_name, offset);
		rbp = (uint64_t*)*rbp; // jump to previous stack frame
	}

	kprintf("======== End of Backtrace ========\n");
}

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
