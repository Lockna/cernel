#pragma once

/**
 *	@file vmm.h
 *	@brief Function prototypes, structs, macros used by the whole paging algorithm
 */

#include <stivale/stivale.h>
#include <stdint.h>

/// Kernel offset from beginn of ram
#define KERNEL_PHYS_OFFSET ((uintptr_t)0xFFFFFFFF80000000)
/// Higher Half
#define HIGHER_HALF ((uintptr_t)0xFFFF800000000000)


/**
 *	@brief Used to access the single parts of a splitted virtual address
 */
struct AddressIndexer {
	uint64_t pml4 : 9;    ///< index of the page directory pointer table for this virtual address
	uint64_t pml3 : 9;    ///< index of the page directory table for this virtual address
	uint64_t pml2 : 9;    ///< index of the page table for this virtual address
	uint64_t pml1 : 9;    ///< index of the page table for this virtual address
	uint64_t offset : 12; ///< offset in the page of the virtual address
} __attribute__ ((__packed__));

/**
 *	@brief data structure used by the mmu to enable virtual memory
 */
struct PageTableEntry {
	uint64_t present : 1; ///< marks if Entry is present or not
	uint64_t writable : 1; ///< marks if it's writable or not
	uint64_t user_access : 1; ///<
	uint64_t write_through : 1; ///<
	uint64_t disable_cache : 1; ///<
	uint64_t accessed : 1; ///<
	uint64_t dirty : 1; ///<
	uint64_t huge_page : 1; ///<
	uint64_t global : 1; ///< marks if this Entry is visible to all pagetables
	uint64_t custom0 : 3; ///< Free to use bits
	uint64_t addr : 52; ///< physical address
} __attribute__ ((__packed__));

/**
 *	@brief PageTable which can be loaded and used, consists of 512 entries
 */
struct PageTable {
	struct PageTableEntry entries[512];
}__attribute__((aligned(0x1000)));

/// Make the pagetable of the kernel visible outside
extern struct PageTable *pt_kernel;

/**
 *	@brief Creates a struct that has the single parts of a virtual address
 *	@param indexer The indexer the single parts should get written into
 *	@param virt_addr virtual address which should get split up
 */
void create_address_indexer(struct AddressIndexer *indexer, uintptr_t virt_addr);

/**
 *	@brief Translates a virtual_address into a physical address
 *	@param page_table pagetable which should get used to translate
 *	@param virt_addr virtual address which should get translated
 *	@return the physical address as uintptr_t
 */
uintptr_t vmm_translate(struct PageTable *page_table, uintptr_t virt_addr);

/**
 *	@brief Initializes the virtual memory manager and identity maps the address space, maps the kernel to it's offset and higher half and maps the framebuffer
 *	@param stivale stivale struct which holds the memory map needed for identity mapping
 */
void vmm_init(struct stivale_struct *stivale);

/**
 *	@brief Maps an address
 *	@param page_table page table where the mapping should take place
 *	@param virt_addr virtual address which should get mapped
 *	@param phys_addr physical address where the virtual address should get mapped to
 */
void vmm_map(struct PageTable *page_table, uintptr_t virt_addr, uintptr_t phys_addr);

/**
 *	@brief Unmaps an address
 *	@param page_table page table where the unmapping should take place
 *	@param virt_addr virtual address which should get unmapped
 */
void vmm_unmap(struct PageTable *page_table, void *virt);

/**
 *	@brief Creates a new address space
 *	@return new PageTable
 */
struct PageTable *vmm_create_new_address_space();

/**
 *	@brief Reads the value in cr3 register, aka the current page table
 *	@return the address to the current page table
 */
void *cr3_read();

/**
 *	@brief Sets the cr3 to a arbitrary value
 *	@param value the value the cr3 should get set to
 */
void cr3_set(uintptr_t value);

/**
 *	@brief Flushes the tlb
 *	@param addr The address of the tlb
 */
void tlb_flush(unsigned long addr);