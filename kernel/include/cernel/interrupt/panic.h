#pragma once

#include <cernel/interrupt/handler.h>

__attribute__((noreturn)) 
void panic(cpu_register_state_t *state, char *error_message);

__attribute__((noreturn))
void generic_panic(char *error_message);