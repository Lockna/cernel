#pragma once

#include <stivale/stivale.h>

#define SYMBOLS_MODULE_NAME "debug_symbols"

struct debug_symbol {
    uint64_t address;
    char *name;
};

void load_debug_symbols(struct stivale_module *modules, uint64_t modules_count);

// takes an address, return name and offset from start of function
const char* get_symbol_name(uint64_t address, uint64_t *offset);