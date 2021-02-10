#include <debug/debug.h>

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
