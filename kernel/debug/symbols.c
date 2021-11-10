#include <debug/symbols.h>
#include <cernel/lib/string.h>
#include <cernel/lib/alloc.h>
#include <cernel/lib/memory.h>
#include <cernel/lib/print.h>
#include <cernel/interrupt/panic.h>
#include <debug/debug.h>

char *symbols_file = NULL;

struct debug_symbol* function_symbols = NULL;
int symbols_count = 0;

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

    // calculate the number of function symbols
    while (symbols_file[i] != '\0') {
        if (symbols_file[i] == '\n')
            symbols_count++;
        i++;
    }    

    kprintf("symbol count: %u\n", symbols_count);

    function_symbols = kmalloc(symbols_count * sizeof(struct debug_symbol));

    char *cursor = symbols_file;

    for (i = 0; i < symbols_count; i++) {
        char *nextLine = NULL;

        int counter = 0;

        while (cursor[counter] != '\n')
            counter++;
        
        cursor[counter] = '\0';
        nextLine = &cursor[counter+1];

        char *after;

        // TODO: remove this shitty hack

        uint64_t address = strtol(++cursor, &after, 16);

        function_symbols[i].address = address;
        function_symbols[i].name = ++after;
        function_symbols[i].address |= 0xf000000000000000;

        cursor = nextLine;
    }
}

const char* get_symbol_name(uint64_t address, uint64_t *offset)
{
    if (function_symbols == NULL) {
        *offset = 0;
        return NULL;
    }
    
    for (int i = 0; i < symbols_count; i++) {
        dbg_printf("%x == %x\n", address, function_symbols[i].address);
        if (address < function_symbols[i].address) {      
            i--;
            *offset = address - function_symbols[i].address;
            return function_symbols[i].name;
        }
    }
    

    return NULL;
}