#pragma once

/**
 *	@file pmm.h
 *	@brief Function prototypes, struct and macros used by the memory allocator
 */

#include <stdint.h>
#include <stivale/stivale.h>

#define PAGE_SIZE 4096

/**
 *	@brief struct for usable ram memory map entries
 */
struct usable_ram {
	uint64_t base; ///< base address of the memory entry
	uint64_t page_count; ///< The size of the memory "segment" in pages
	uint64_t length; ///< The length of the memory part in bytes
};

/**
 *	@brief Initializes the bitmap
 *	@param memory_map_addr pointer to memory map provided by stivale
 *	@param count number of memory_map entries
 */
void pmm_init(struct stivale_mmap_entry *memory_map_addr, uint32_t count);

/**
 *	@brief Allocates a pageframe
 *	@return address to pageframe as uintptr_t
 */
uintptr_t pmm_alloc();

/**
 *	@brief Allocates a pageframe and initializes it with 0
 *	@return address to pageframe as uintptr_t
 */
uintptr_t pmm_allocz();

/**
 *	@brief frees a pageframe and mark it as free to use
 *	@param page_addr pageframe to be freed
 */
void pmm_free(uintptr_t page_addr);