#pragma once

#include <stivale/stivale.h>
#include <stdint.h>

#define PAGING_LVL 4
#define KERNEL_PHYS_OFFSET ((uintptr_t)0xFFFFFFFF80000000)
#define HIGHER_HALF ((uintptr_t)0xFFFF800000000000)

struct AddressIndexer {
	uint64_t page_directory_pointer_index : 9;
	uint64_t page_directory_index : 9;
	uint64_t page_table_index : 9;
	uint64_t page_index : 9;
} __attribute__ ((__packed__));

struct PageTableEntry {
	uint64_t present : 1;
	uint64_t writable : 1;
	uint64_t user_access : 1;
	uint64_t write_through : 1;
	uint64_t disable_cache : 1;
	uint64_t accessed : 1;
	uint64_t dirty : 1; 
	uint64_t huge_page : 1;
	uint64_t global : 1;
	uint64_t custom0 : 3;
	uint64_t addr : 52;
} __attribute__ ((__packed__));

struct PageTable {
	struct PageTableEntry entries[512];
}__attribute__((aligned(0x1000)));

void create_address_indexer(struct AddressIndexer *indexer, uintptr_t virt_addr);

void *vmm_translate(struct PageTable *page_table, void *virt_addr);
void vmm_init(struct stivale_mmap_entry *mmap, uint64_t mmap_count);
void vmm_map(struct PageTable *page_table, uintptr_t virt_addr, uintptr_t phys_addr);
void vmm_unmap(struct PageTable *page_table, void *virt);
struct PageTable *vmm_create_new_address_space();

void *cr3_read();
void cr3_set(uintptr_t value);
void tlb_flush(unsigned long addr);
