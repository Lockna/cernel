#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

#define LINES 25
#define COLUMNS_PER_LINE 80
#define BYTES_PER_ELEMENT 2
#define LINESIZE = BYTES_PER_ELEMENT * COLUMNS_PER_LINE
#define SCREENSIZE LINES * LINESIZE

int load_fb_driver(uint64_t *fb_adress, 
                uint16_t framebuffer_width,
                uint16_t framebuffer_height, 
                uint16_t framebuffer_bpp,
                uint16_t framebuffer_pitch);
void putc(char c);
void puts(char *str);

#endif
