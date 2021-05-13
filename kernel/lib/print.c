/**
 *	@file print.c
 *	@brief Function for printing formatted strings to the framebuffer
 *
 *  In the kernel you can simply use kprintf to print something to screen
 *	kprintf calls intern_printf which specifies the format parameters used in the format string
 *	and converts the given variables to strings so they can be printed easily to screen.
 */

#include <cernel/lib/print.h>
#include <cernel/drivers/fb/framebuffer.h>
#include <stdint.h>

void kprintf(char *format, ...)
{
	va_list args;
	va_start(args, format);
	
	intern_printf(format, args, putc);

	va_end(args);
}

void intern_printf(char *format, va_list args, void (*putc_ptr)(char c))
{
	while (*format != '\0') {
		char c = *format;
		if (c != '%') {
			// the current char is not a '%' or it is the last char of the string
			// print it and then check the next one
			
			putc_ptr(*format);
			format++;
		} else {
			// the current char is a '%'
			// all cases will be handled here

			format++; // skipping over '%'

			// check if width is specified and set it
			int width = 0;
			
			while (isDigit(*format) && *format != '\0') {
				width *= 10;
				width += (*format) - '0';
				format++;
			}			

			// handle all cases here
			switch (*format) {
			case '%':
				putc_ptr(*format);
				format++;
				break;
			case 'd':
			case 'i':
				print_int64(va_arg(args, int64_t), width, putc_ptr);
				format++;
				break;
			case 'u':
				print_uint64(va_arg(args, uint64_t), width, putc_ptr);
				format++;
				break;
			case 's':
				print_string(va_arg(args, char*), width, putc_ptr);
				format++;
				break;
			case 'c':
				c = va_arg(args, int);
				print_char(c, width, putc_ptr);
				format++;
				break;
			case 'x':
				print_hex(va_arg(args, uint64_t), width, 0, putc_ptr);
				format++;
				break;
			case 'p':
				print_hex(va_arg(args, int64_t), width, 1, putc_ptr);
				format++;
				break;
			case 'b':
				print_bin(va_arg(args, uint64_t), width, putc_ptr);
				format++;
				break;
			} 
		}
	}
}

void print_bin(uint64_t num, int width, void(*putc_ptr)(char c))
{
	char arr[65];
	arr[64] = '\0';
	char *out = arr + 63;

	if (num == 0) {
		out--;
		*out = '0';
	}

	while (num > 0) {
		out--;
		*out = (num % 2) + '0';
		num /= 2;
	}

	int length = strlen(out);

	while (length < width) {
		length++;
		putc_ptr(' ');
	}

	
	while (*out != '\0') {
        putc_ptr(*out);
        out++;
	}
}

char hex_to_ascii[] = "0123456789abcdef";

char *num_to_ascii_hex(char *output, uint64_t num)
{
	*output = '\0';
	
	if (num == 0) {
		output--;
		*output = '0';
	}

	while (num != 0) {
		int current = num % 16;
		num /= 16;
		output--;
		*output = hex_to_ascii[current];
	}

	return output;
}

void print_hex(uint64_t num, int width, int leadingZeros, void (*putc_ptr)(char c))
{
	char buffer[17];
	char *output = num_to_ascii_hex(buffer+16, num);

	int length = strlen(output) + 2;

	if (leadingZeros == 1)
		length = 18;

	int diff = width - length;

	while (diff --> 0) {
		putc_ptr(' ');
	}

	putc_ptr('0');
	putc_ptr('x');
	
	length -= 2; // subtract the already printed "0x"

	if (leadingZeros == 1) {
		length = strlen(output);

		while (length < 16) {
			putc_ptr('0');
			length++;
		}
	}

	while (*output != '\0') {
        putc_ptr(*output);
        output++;
	}	
}

void print_char(char ch, int width, void (*putc_ptr)(char c))
{
	width -= 1;

	while (width --> 0) {
		putc_ptr(' ');
	}

	putc_ptr(ch);
}

void print_string(char *output, int width, void (*putc_ptr)(char c))
{
	int length = strlen(output);
	int diff = width - length;

	while (diff --> 0) {
		putc_ptr(' ');
	}

	while (*output != '\0') {
        putc_ptr(*output);
        output++;
    }
}

char *uint64_to_ascii_dec(char *output, uint64_t num)
{
	*output = '\0';

	if (num == 0) {
		output--;
		*output = '0';	
	}

	while (num != 0) {
		output--;
		*output = num % 10 + '0';
		num /= 10;
	}

	return output;
}

void print_uint64(uint64_t num, int width, void (*putc_ptr)(char c))
{
	char arr[21];
	char *output = uint64_to_ascii_dec(arr+20, num);

	int length = strlen(output);

	int diff = width - length;

	while (diff --> 0) {
		putc_ptr(' ');
	}

	while (*output != '\0') {
		putc_ptr(*output);
		output++;
	}
}


char *int64_to_ascii_dec(char *output, int64_t num)
{
	*output = '\0';
	int isNegative = 0;

	if (num < 0) {
		isNegative = 1;
		num = -num;
	}

	if (num == 0) {
		output--;
		*output = '0';	
	}

	while (num != 0) {
		output--;
		*output = num % 10 + '0';
		num /= 10;
	}

	if (isNegative == 1) {
		output--;
		*output = '-';
	}

	return output;
}

void print_int64(int64_t num, int width, void (*putc_ptr)(char c))
{
	char arr[21];
	char *output = int64_to_ascii_dec(arr+20, num);

	int length = strlen(output);

	int diff = width - length;

	while (diff --> 0) {
		putc_ptr(' ');
	}

	while (*output != '\0') {
		putc_ptr(*output);
		output++;
	}
}

int strlen(char *str)
{
	int ret = 0;
	
	while(*str != '\0') {
		ret++;
		str++;
	}

	return ret;
}

int isDigit(char ch)
{
	return ch >= '0' && ch <= '9';
}