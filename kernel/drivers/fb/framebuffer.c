#include <drivers/fb/framebuffer.h>
#include <stdint.h>
#include <drivers/fb/font.h>

uint32_t cursor_loc = 0;
uint8_t *framebuffer = 0;
uint16_t width = 0;
uint16_t height = 0;
uint16_t bpp = 0;
uint16_t pitch = 0;

int load_fb_driver(uint64_t *fb_adress, 
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
	return 0;
}

void putc(char c) 
{	
	int index = 0;

	if (c != ' ') {
		index = (c - 32) * 16;
	}

	for (int i = index; i < (index+16); i++) {

		char tmp = font_bitmap[i];

		for (int j = 0; j < 8; j++) {

			if (tmp & (1 << 7-j)) {
				framebuffer[cursor_loc] = 0xff;
				framebuffer[cursor_loc+1] = 0xff;
				framebuffer[cursor_loc+2] = 0xff;
			}
			cursor_loc += (bpp/8);
		}
		cursor_loc -= bpp;
		cursor_loc += pitch;
	}

	cursor_loc += bpp;
	cursor_loc += (bpp/8);

	cursor_loc -= pitch*16;
}

void puts(char *str) {

	int index = 0;

	while (str[index] != '\0') {
		putc(str[index++]);
	}

}