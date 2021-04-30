#include <cernel/mm/vmm.h>
#include <stddef.h>
#include <stdint.h>

void create_address_indexer(struct AddressIndexer *indexer, uintptr_t virt_addr) {

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
