#pragma once

/**
 *	@file print.h
 *	@brief Function prototypes for internal print functions used by the kernel
 *
 *  kprintf results into a call to putc which prints all the characters.
 *  Most other functions are used to convert values to strings so they can be printed easily
 */

#include <stdarg.h>
#include <stdint.h>

/**
 *  @brief parses and prints the format string
 *  @param format the format string
 *  @param args the list of arguments
 *  @param putc_ptr function pointer to print a char 
 */
void intern_printf(char *format, va_list args, void (*putc_ptr)(char c));

/**
 *  @brief calls internal_printf
 *  @param format String how the output should look like
 */
void kprintf(char *format, ...);

/*
 * @brief prints a number as binary
 * @param num the number which gets printed as binary
 * @param width minimal width of the output
 * @param putc_ptr functionpointer to write each char to
 */
void print_bin(uint64_t num, int width, void(*putc_ptr)(char c));

/**
 *  @brief converts a unsigned integer to ASCII decimal string
 *  @param output a pointer to the location where the string should be written to
 *  @param num the number which should get converted to ASCII decimal string
 *  @return a pointer to the location where the ASCII decimal string is stored
 */
char *uint64_to_ascii_dec(char *output, uint64_t num);

/**
 *  @brief calls the specified function with the ASCII string of the unsigned number
 *  @param num the number which should get printed
 *  @param width minimum length of the output, padded with spaces if neccesary
 *  @param putc_ptr functionpointer to write each char to
 */
void print_uint64(uint64_t num, int width, void (*putc_ptr)(char c));

/**
 *  @brief calls the specified function with the ASCII string of the signed number
 *  @param num the number which should get printed
 *  @param width minimum length of the output, padded with spaces if neccesary
 *  @param putc_ptr functionpointer to write each char to
 */
void print_int64(int64_t num, int width, void (*putc_ptr)(char c));

/**
 *  @brief converts a signed integer to ASCII decimal string
 *  @param output a pointer to the location where the string should be written to
 *  @param num the number which should get converted to ASCII decimal string
 *  @return a pointer to the location where the ASCII decimal string is stored
 */
char *int64_to_ascii_dec(char *output, int64_t num);

/**
 *  @brief writes a string to putc_ptr
 *  @param ouput string which should get printed
 *  @param width minimal length of the output
 *  @param putc_ptr functionpointer to write each char to
 */
void print_string(char *output, int width, void (*putc_ptr)(char c));

/**
 *  @brief writes a char to putc_ptr
 *  @param ouput string which should get printed
 *  @param width minimal length of the output
 *  @param putc_ptr functionpointer to write each char to
 */
void print_char(char ch, int width, void (*putc_ptr)(char c));

/**
 *  @brief writes a hex number to putc_ptr
 *  @param num number which gets printed as hex
 *  @param width minimal length of the output
 *  @param putc_ptr functionpointer to write each char to
 */
void print_hex(uint64_t num, int width, int leadingZeros, void (*putc_ptr)(char c));

/**
 *  @brief converts a signed integer to ASCII hex string
 *  @param output a pointer to the location where the string should be written to
 *  @param num the number which should get converted to ASCII hex string
 *  @return a pointer to the location where the ASCII hex string is stored
 */
char *num_to_ascii_hex(char *output, uint64_t num);

void print_rgb(uint8_t r, uint8_t g, uint8_t b, char *format, ...);