#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

/**
 *	@file framebuffer.h
 *	@brief Function prototypes for the framebuffer
 *
 *  The functions are used to write on the framebuffer and thus resulting into characters 
 *  being printed on the screen
 */

#include <stdint.h>

/// Make color publicly visible
extern uint32_t color;

/**
 *	@brief Sets the global variables to their corrosponding value
 *	@param fb_address Address of the framebuffer
 *	@param framebuffer_width Width of the framebuffer in pixels
 *	@param framebuffer_height Heigh of the framebuffer in pixels
 *	@param framebuffer_bpp Bits per pixel, see how many bits are needed for one pixel
 *	@param framebuffer_pitch Width of the framebuffer in bytes
 *	@return 0 at success, since nothing is done, that could fail, it just returns 0
 */
uint8_t load_fb_driver(uint8_t *fb_adress, 
                uint16_t framebuffer_width,
                uint16_t framebuffer_height, 
                uint16_t framebuffer_bpp,
                uint16_t framebuffer_pitch);

/**
 *	@brief Prints a single character to the pixelframebuffer
 *	@param c char to be printed
 */
void putc(char c);

/**
 *	@brief Writes a string to the pixelframebuffer using putc
 *	@param str string to be printed
 */
void puts(char *str);

/**
 *  @brief Cleares the framebuffer
 */
void fb_clear();

void fb_set_color(uint8_t r, uint8_t g, uint8_t b);

void fb_activate_double_buffering();
void fb_flush();
#endif
