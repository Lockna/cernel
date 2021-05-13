#pragma once

/**
 *	@file panic.h
 *	@brief Panic function prototypes are placed here
 *
 *  The kernel calls the panic function when an error occured which cannot be fixxed by the kernel
 *  and thus leading into a so called panic-screen and crash of the whole operating system.
 *  If panic is called without a happening interrupt right before, you can just use generic_panic
 *  and pass a error message.
 */


#include <cernel/interrupt/handler.h>

/**
 *  @brief Dumbs the registers to screen and then halt execution
 *  @param state A struct with the general purpose registers of the cpu
 *  @param error_message A message that will be shown on screen
 */
__attribute__((noreturn)) 
void panic(cpu_register_state_t *state, char *error_message);

/**
 *  @brief Gets a dumb of the registers and saves it into a struct, then calls panic with the right params
 *  @param error_message A message that will be shown on screen
 */
__attribute__((noreturn))
void generic_panic(char *error_message);