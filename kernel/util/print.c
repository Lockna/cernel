#include <cernel/util/print.h>

void printf(char *format, ...)
{
	va_list args;
	va_start(args, format);
	
	intern_printf(format, &args, putc);

	va_end(args);
}

void intern_printf(char *format, va_list *args, void (*putc)(char c))
{
	while (format[0] != '\0') {
		putc(format[0]);
		format++;
	}
}
