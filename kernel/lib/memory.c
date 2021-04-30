#include <cernel/util/memory.h>
#include <stivale/stivale.h>
#include <cernel/util/print.h>

uint64_t get_memory_size(struct stivale_mmap_entry *memory_map_addr, uint32_t count) {

    uint64_t memory_size = 0;

    for (uint32_t i = 0; i < count; i++) {
        memory_size += memory_map_addr[i].length;

    }

    return memory_size;

}