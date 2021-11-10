#include <debug/symbols.h>
#include <cernel/lib/string.h>
#include <cernel/lib/alloc.h>
#include <cernel/lib/memory.h>
#include <cernel/lib/print.h>
#include <cernel/interrupt/panic.h>

char *symbols_file = NULL;

struct debug_symbol* function_symbols = NULL;

void load_debug_symbols(struct stivale_module *modules, uint64_t modules_count)
{
    for (size_t i = 0; i < modules_count && symbols_file == NULL; i++) {
        if (strcmp(modules[i].string, SYMBOLS_MODULE_NAME) == 0) {
            // found correct entry
            uint64_t length = modules[i].end - modules[i].begin;
            length++; // '\0' at the end of a string

            symbols_file = kmalloc(length);

            memcpy((uint8_t*)symbols_file, (uint8_t*)modules[i].begin, length - 1);

            symbols_file[length - 1] = '\0';
        }
    }

    if (symbols_file == NULL)
        generic_panic("symbols file could not be loaded");
    
    // parse the symbol file and add every symbol to the symbols array

    int i = 1;
    int symbols_count = 0;

    // calculate the number of function symbols
    while (symbols_file[i] != '\0') {
        if (symbols_file[i] == '\n')
            symbols_count++;
        i++;
    }    

    kprintf("symbol count: %u\n", symbols_count);

    function_symbols = kmalloc(symbols_count * sizeof(struct debug_symbol));

    i = 0;


}

const char* get_symbol_name(uint64_t address, uint64_t *offset)
{
    if (function_symbols == NULL) {
        *offset = 0;
        return NULL;
    }


}