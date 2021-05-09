#pragma once

#include <stivale/stivale.h>
#include <stdint.h>

#define KERNEL_PHYS_OFFSET ((uintptr_t)0xFFFFFFFF80000000)
#define HIGHER_HALF ((uintptr_t)0xFFFF800000000000)

struct AddressIndexer {
	uint64_t pml4 : 9;
	uint64_t pml3 : 9;
	uint64_t pml2 : 9;
	uint64_t pml1 : 9;
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
	uint64_t addr : 52;
} __attribute__ ((__packed__));

struct PageTable {
	struct PageTableEntry entries[512];
}__attribute__((aligned(0x1000)));

extern struct PageTable *pt_kernel;

void create_address_indexer(struct AddressIndexer *indexer, uintptr_t virt_addr);

uintptr_t vmm_translate(struct PageTable *page_table, uintptr_t virt_addr);
void vmm_init(struct stivale_struct *stivale);
void vmm_map(struct PageTable *page_table, uintptr_t virt_addr, uintptr_t phys_addr);
void vmm_unmap(struct PageTable *page_table, void *virt);
struct PageTable *vmm_create_new_address_space();

void *cr3_read();
void cr3_set(uintptr_t value);
void tlb_flush(unsigned long addr);
