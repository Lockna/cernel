#include <cernel/mm/vmm.h>
#include <stddef.h>
#include <stdint.h>

void create_address_indexer(struct AddressIndexer *indexer, uintptr_t virt_addr) {

	indexer->sign_extension = virt_addr >> 48;
	indexer->lvl4 = virt_addr = virt_addr >> 39 & 0b111111111;
	indexer->lvl3 = virt_addr = virt_addr >> 30 & 0b111111111;
	indexer->lvl2 = virt_addr = virt_addr >> 21 & 0b111111111;
	indexer->lvl1 = virt_addr = virt_addr >> 12 & 0b111111111;
	indexer->offset = virt_addr = virt_addr & 0b111111111111;

}

void *vmm_translate(struct PageTable *pageTable, void *virt_addr) {
	return NULL;
}

void vmm_init(struct stivale_mmap_entry *mmap, uint64_t mmap_count) {
	// Creating a new page map for the kernel and set cr3 to it
}

void vmm_map(struct PageTable *pageTable, uintptr_t virt, uintptr_t phys) {

}

void vmm_unmap(struct PageTable *pageTable, void *virt) {

}

struct PageTable *vmm_create_new_address_space() {
	return NULL;
}

void *cr3_read() {
	void *ret;
	asm("\t mov %%cr3,%0" : "=r"(ret));

	return ret;
}

void cr3_set(uintptr_t value) {
	asm volatile("mov %0, %%cr3" : : "r"(value));
}

void tlb_flush(unsigned long addr) {
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}
