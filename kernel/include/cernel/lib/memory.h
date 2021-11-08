#pragma once

/**
 *	@file memory.h
 *	@brief Various function prototypes for memory functions
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stivale/stivale.h>

/**
 *  @brief Calculates the available memory size
 *  @param memory_map_addr Address of a memory map delivered by the bootloader
 *  @param count entries in the memory map
 *  @return memory size in bytes
 * 
 *  Calculates the memory size using the memory map provided by the bootloader
 *  It uses count to iterate through the memory map and 
 *  just adds the length of the usable entries together, then returning the result.
 */
size_t get_memory_size(struct stivale_mmap_entry *memory_map_addr, uint32_t count);

/**
 *  @brief Sets the memory at the specified address to a specific value n times in bytes
 *  @param ptr Points to the memory where it should be written
 *  @param value The value that should be written into memory
 *  @param count How much should be written
 *  @return just returns the pointer
 */
void *memset(uint8_t *ptr, uint8_t value, size_t count);

/**
 *  @brief Sets the memory at the specified address to a specific value n times in 8 bytes
 *  @param ptr Points to the memory where it should be written
 *  @param value The value that should be written into memory
 *  @param count How much should be written
 *  @return just returns the pointer
 */
void *memset64(uint64_t *ptr, uint64_t value, size_t count);

bool memcmp(void *p1, void *p2, size_t count);

void *memcpy(uint8_t *dst, uint8_t *src, size_t n);