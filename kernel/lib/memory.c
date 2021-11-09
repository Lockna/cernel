/**
 *	@file memory.c
 *	@brief Various memory functions
 */

#include <cernel/lib/memory.h>
#include <stivale/stivale.h>

size_t get_memory_size(struct stivale_mmap_entry *memory_map_addr, uint32_t count) {

    size_t memory_size = 0;

    for (uint32_t i = 0; i < count; i++) {
        memory_size += memory_map_addr[i].length;
    }

    return memory_size;
}

void *memset(uint8_t *ptr, uint8_t value, size_t count) {

	for (size_t i = 0; i < count; i++) {
		ptr[i] = value;
	}

	return ptr;
}

void *memset64(uint64_t *ptr, uint64_t value, size_t count) {

	for (size_t i = 0; i < count; i++) {
		ptr[i] = value;
	}

	return ptr;
}

bool memcmp(void *p1, void *p2, size_t count)
{
	if (p1 == NULL || p2 == NULL)
		return false;
	
	uint8_t *ptr1 = (uint8_t *)p1;
	uint8_t *ptr2 = (uint8_t *)p2;

    for (size_t i = 0; i < count; i++) {
        if (ptr1[i] != ptr2[i])
            return false;
    }

    return true;
}

volatile void *memcpy(volatile uint8_t *dst, volatile uint8_t *src, size_t n) 
{
	for (size_t i = 0; i < n; i++) {
		dst[i] = src[i];
	}

	return dst;
}