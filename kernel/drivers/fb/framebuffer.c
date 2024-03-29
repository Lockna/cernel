/**
 *	@file framebuffer.c
 *	@brief Framebuffer driver
 *
 *	These functions are used to write to the framebuffer
 *	and further print chars.
 *	The framebuffer is capable of printing in RGB but we are writing 0xFFFFFF so we got a clean white here
 */

#include <cernel/drivers/fb/framebuffer.h>
#include <cernel/drivers/fb/font.h>
#include <cernel/lib/alloc.h>
#include <cernel/lib/memory.h>
#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>
#include <cernel/lib/print.h>

uint64_t cursor_loc = 0;
volatile uint8_t *framebuffer = 0;
uint16_t width = 0;
uint16_t height = 0;
uint16_t bpp = 0;
uint16_t pitch = 0;
uint32_t fb_length_in_bytes = 0;
uint16_t char_per_line = 0;
uint16_t char_current_line = 0;

uint32_t color = 0x00ffffff;

volatile uint8_t *double_buffer = NULL;

volatile uint8_t *buffer = NULL;

bool buffering_active = false;

uint8_t load_fb_driver(uint8_t *fb_address, 
				uint16_t framebuffer_width,
				uint16_t framebuffer_height, 
                uint16_t framebuffer_bpp,
                uint16_t framebuffer_pitch) 
{
	framebuffer = fb_address;
	width = framebuffer_width;
	height = framebuffer_height;
	pitch = framebuffer_pitch;
	bpp = framebuffer_bpp;
	fb_length_in_bytes = height * pitch;
	char_per_line = framebuffer_width / 9;
	buffer = framebuffer;

	return 0;
}

void putc(char c) 
{	
	uint16_t index = 0;

	if (cursor_loc >= fb_length_in_bytes-pitch*17) {
		cursor_loc -= 17*pitch;
  		// for (int line = 17; line < height; line++) {
    	// 	for (int x = 0; x < width; x++) {
      	// 		(buffer)[(line-17) * (pitch / 4) + x] = (buffer)[line * (pitch/4) + x];
    	// 	}
  		// }
		memcpy(buffer, buffer + pitch*17, fb_length_in_bytes - pitch*17);

  		for (size_t i = 0; i < pitch*16; i++) {
    		buffer[cursor_loc + i] = 0x0;
  		}
	}

	if (c == '\n') {
		/// Subtracts the offset from the left edge, so the char is at the beginning of the line
		cursor_loc -= cursor_loc % pitch;
		/// Adds the size of a char, with one line as buffer, so they don't stick together
		cursor_loc += pitch * 17; ///< 16, the "size" of a char + 1 pixelline
		char_current_line = 0;
		fb_flush();
		return;
	}

	if (c == '\t') {
		puts("    ");
		return;
	}

	/// In our implementation \r deletes the whole lin, maybe reworked later
	if (c == '\r') {
		cursor_loc -= cursor_loc % pitch;

		for (uint32_t i = 1; i  <= pitch*16; i++) {
			buffer[cursor_loc + i] = 0x0;
		}
		return;
	}
	
	if (char_per_line == char_current_line) {
		char_current_line = 0;
		cursor_loc -= cursor_loc % pitch;
		cursor_loc += pitch * 17;
	}

	/*
	 *	First 32 ascii chars are skipped in the bitmap array
	 *  calculates the correct index for the char
	 */
	if (c != ' ') {
		index = (c - 32) * 16;
	}

	char_current_line++;		

	for (uint16_t i = index; i < (index+16); i++) {

		/// Gets the n-th line of the bitmap
		char bitmap = font_bitmap[i];

		for (uint8_t j = 0; j < 8; j++) {

			/// Iterate through the single bits of the n-th bitmap line
			if (bitmap & (1 << (7-j))) {
				buffer[cursor_loc] = color & 0xff;
				buffer[cursor_loc+1] = (color >> 0x8) & 0xff;
				buffer[cursor_loc+2] = (color >> 0xf) & 0xff;
			}
			/// Add the length of a single pixel to the cursor, so it won't overwrite the already drawn pixel
			cursor_loc += (bpp/8);
		}
		/// Resets the cursor to the beginning of the char but in the next line (pixelline)
		cursor_loc -= bpp;
		cursor_loc += pitch;
	}

	/// Sets the cursor to the next chars and adds a little space, so it looks neat
	cursor_loc += bpp;
	cursor_loc += (bpp/8);

	/// Cursor is in the bottom right corner of the bitmap, so set it to top again, 
	/// else the next char would be in the right down diagonal to the already written one
	cursor_loc -= pitch*16;
}

void puts(char *str) 
{
	/// Sets the index of the string to zero
	uint32_t index = 0;

	/// Iterating through the string until a nullbyte is hit
	while (str[index] != '\0') {
		/// Printing the char on the specified index
		putc(str[index++]);
	}
}

void fb_clear() 
{
	/// Clearing the whole framebuffer
	for (size_t i = 0; i < fb_length_in_bytes; i++) {
		framebuffer[i] = 0;
	}

	/// Setting the cursor to position 0
	cursor_loc = 0;
}

void fb_set_color(uint8_t r, uint8_t g, uint8_t b) 
{
	/// First clearing the color variable
	color = 0;

	/// Set the color accordingly
	color |= r << 16;
	color |= g << 8;
	color |= b;
}

void fb_activate_double_buffering()
{
	buffering_active = true;

	double_buffer = kmalloc(fb_length_in_bytes);

	buffer = double_buffer;

	memcpy(double_buffer, framebuffer, fb_length_in_bytes);
}

void fb_flush()
{
	if (buffering_active) {
		memcpy(framebuffer, double_buffer, fb_length_in_bytes);
	}
}