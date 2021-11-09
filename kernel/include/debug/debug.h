#pragma once

#include <stdint.h>

/** 
 *  @file debug.h
 *  @brief Function prototypes for debug print functions to qemu console
 *
 *  This file contains the function prototypes of the debugging area of the kernel.
 */

/**
 *  @brief prints a string to the QEMU console
 *  @param str the string which gets printed
 *  @return void
 */
void dbgln(char *str);

/**
 *  @brief prints a char to the QEMU console
 *  @param c the char which gets printed
 *  @return void
 */
void dbgc(char c);

/**
 *  @brief prints formatted string to QEMU console
 *  @param format Holds the format string
 *  @return void
 */
void dbg_printf(char *format, ...);

void print_stacktrace(uint64_t rip_value, uint64_t rbp_value);
