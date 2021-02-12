#include <lib/print.h>

void intern_printf(char *format, va_list *args, void (*putc)(char c))
{
	while (format[0] != '\0') {
		putc(format[0]);
		format++;
	}
}
