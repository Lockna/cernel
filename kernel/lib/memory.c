#include <cernel/lib/memory.h>
#include <stivale/stivale.h>

size_t get_memory_size(struct stivale_mmap_entry *memory_map_addr, uint32_t count) {

    size_t memory_size = 0;

    for (uint32_t i = 0; i < count; i++) {
        memory_size += memory_map_addr[i].length;

    }

    return memory_size;

}