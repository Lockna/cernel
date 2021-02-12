#pragma once

#include <stdarg.h>

// write the output string to `putc` function
void intern_printf(char *format, va_list *args, void (*putc)(char c));
