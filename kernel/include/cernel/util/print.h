#pragma once

#include <stdarg.h>
#include <stdint.h>

// write the output string to `putc` function
void intern_printf(char *format, va_list args, void (*putc)(char c));

/**
 *  @brief calls internal_printf
 *  @param format String how the output should look like
 *  @return void
 */
void printf(char *format, ...);

int isDigit(char ch);
char *uint64_to_ascii_dec(char *output, uint64_t num);
void print_uint64(uint64_t num, int width, void (*putc_ptr)(char c));
void print_int64(int64_t num, int width, void (*putc_ptr)(char c));
char *int64_to_ascii_dec(char *output, int64_t num);
int strlen(char *str);
void print_string(char *output, int width, void (*putc_ptr)(char c));
void print_char(char ch, int width, void (*putc_ptr)(char c));
void print_hex(int64_t num, int width, int leadingZeros, void (*putc_ptr)(char c));
char *num_to_ascii_hex(char *output, int64_t num);
