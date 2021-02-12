#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

/**
 *	@brief Sets the global variables to their corrosponding value
 *	@param fb_adress
 *	@param framebuffer_width
 *	@param framebuffer_height
 *	@param framebuffer_bpp
 *	@param framebuffer_pitch
 *	@return 0 at success, since nothing is done, that could fail, it just returns 0
 */
int load_fb_driver(uint64_t *fb_adress, 
                uint16_t framebuffer_width,
                uint16_t framebuffer_height, 
                uint16_t framebuffer_bpp,
                uint16_t framebuffer_pitch);

/**
 *	@brief Prints a single character to the pixelframebuffer
 *	@param c char to be printed
 *	@return void
 */
void putc(char c);

/**
 *	@brief Writes a string to the pixelframebuffer using putc
 *	@param str string to be printed
 *	@return void
 */
void puts(char *str);

#endif
