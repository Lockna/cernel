#pragma once

/**
 *    @brief prints a string to the QEMU console
 *    @param str the string which gets printed
 *    @return void
 */
void dbgln(char *str);

/**
*    @brief prints a char to the QEMU console
*    @param ch the char which gets printed
*    @return void
*/
void dbgc(char c);

void dbg_printf(char *format, ...);
