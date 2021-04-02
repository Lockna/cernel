#include <cernel/drivers/fb/framebuffer.h>
#include <cernel/drivers/fb/font.h>
#include <stdint.h>
#include <stdarg.h>
#include <cernel/util/print.h>

uint32_t cursor_loc = 0;
volatile uint8_t *framebuffer = 0;
uint16_t width = 0;
uint16_t height = 0;
uint16_t bpp = 0;
uint16_t pitch = 0;
uint16_t char_per_line = 0;
uint16_t char_current_line = 0;

int load_fb_driver(uint8_t *fb_adress, 
				uint16_t framebuffer_width,
				uint16_t framebuffer_height, 
                uint16_t framebuffer_bpp,
                uint16_t framebuffer_pitch) 
{
	framebuffer = fb_adress;
	width = framebuffer_width;
	height = framebuffer_height;
	pitch = framebuffer_pitch;
	bpp = framebuffer_bpp;
	char_per_line = framebuffer_width / 9;

	return 0;
}

void putc(char c) 
{	
	int index = 0;

	if (c == '\n') {
		/// Subtracts the offset from the left edge, so the char is at the beginning of the line
		/// Adds the size of a char, with one line as buffer, so they don't stick together
		cursor_loc -= cursor_loc % pitch;
		cursor_loc += pitch * 17; // <-- 16, the "size" of a char + 1 pixelline
		char_current_line = 0;
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

	for (int i = index; i < (index+16); i++) {

		/// Gets the n-th line of the bitmap
		char bitmap = font_bitmap[i];

		for (int j = 0; j < 8; j++) {

			/// Iterate through the single bits of the n-th bitmap line
			if (bitmap & (1 << (7-j))) {
				framebuffer[cursor_loc] = 0xff;
				framebuffer[cursor_loc+1] = 0xff;
				framebuffer[cursor_loc+2] = 0xff;
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

void puts(char *str) {

	int index = 0;

	while (str[index] != '\0') {
		putc(str[index++]);
	}
}
