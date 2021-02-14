#pragma once

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

/**
 *  @brief checks whether a char is a digit
 *  @param ch the char to check
 *  @return 1 if the char is digit, otherwise 0
 */
int isDigit(char ch);

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
 *  @brief calculates the length of a string
 *  @param str pointer to a string
 *  @return length of the specified string
 */
int strlen(char *str);

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
void print_hex(int64_t num, int width, int leadingZeros, void (*putc_ptr)(char c));

/**
 52  *  @brief converts a signed integer to ASCII hex string
 53  *  @param output a pointer to the location where the string should be written to
 54  *  @param num the number which should get converted to ASCII hex string
 55  *  @return a pointer to the location where the ASCII hex string is stored
 56  */
char *num_to_ascii_hex(char *output, int64_t num);
