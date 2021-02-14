#pragma once

#include <stdarg.h>

// write the output string to `putc` function
void intern_printf(char *format, va_list args, void (*putc)(char c));

/**
 *  @brief calls internal_printf
 *  @param format String how the output should look like
 *  @return void
 */
void printf(char *format, ...);
