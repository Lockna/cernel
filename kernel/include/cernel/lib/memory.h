#pragma once
#include <stddef.h>
#include <stivale/stivale.h>

size_t get_memory_size(struct stivale_mmap_entry *memory_map_addr, uint32_t count);