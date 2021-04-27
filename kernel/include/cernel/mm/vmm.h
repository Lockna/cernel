#pragma once

#include <stivale/stivale.h>

#define PAGING_LVL 4

struct VirtualAddressIndexer {
	uint64_t sign_extension : 16;
	uint64_t lvl4 : 9;
	uint64_t lvl3 : 9;
	uint64_t lvl2 : 9;
	uint64_t lvl1 : 9;
	uint64_t offset : 12;	
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
	uint64_t phys_addr : 52;
} __attribute__ ((__packed__));

struct PageTable {
	struct PageTableEntry entries[512];
}__attribute__((aligned(0x1000)));

void createVirtualAddressIndexer(struct VirtualAddressIndexer *indexer);

void *vmm_translate(struct PageTable *pageTable, void *virt_addr);
void vmm_init(struct stivale_mmap_entry *mmap, uint64_t mmap_count);
void vmm_map(struct PageTable *pageTable, void *virt, void *phys);
void vmm_unmap(struct PageTable *pageTable, void *virt);
struct PageTable *vmm_create_new_address_space();

void *cr3_read();
void cr3_set(void *value);
void tlb_flush();
