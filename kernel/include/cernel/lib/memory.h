#pragma once
#include <stddef.h>
#include <stivale/stivale.h>

size_t get_memory_size(struct stivale_mmap_entry *memory_map_addr, uint32_t count);

void *memset(uint8_t *ptr, uint8_t value, size_t count);
void *memset64(uint64_t *ptr, uint64_t value, size_t count);
