#include <cernel/mm/vmm.h>
#include <cernel/mm/pmm.h>
#include <stddef.h>
#include <stdint.h>

void create_address_indexer(struct AddressIndexer *indexer, uintptr_t virt_addr) 
{
	virt_addr >>= 12;
    indexer->page_index = virt_addr & 0x1ff;
    virt_addr >>= 9;
    indexer->page_table_index = virt_addr & 0x1ff;
    virt_addr >>= 9;
    indexer->page_directory_index = virt_addr & 0x1ff;
    virt_addr >>= 9;
    indexer->page_directory_pointer_index = virt_addr & 0x1ff;
}

void *vmm_translate(struct PageTable *page_table, void *virt_addr) 
{
	return NULL;
}

void vmm_init(struct stivale_mmap_entry *mmap, uint64_t mmap_count) 
{
	// Creating a new page map for the kernel and set cr3 to it
}

void vmm_map(struct PageTable *page_table, uintptr_t virt_addr, uintptr_t phys_addr) 
{
	struct AddressIndexer indexer;
	create_address_indexer(&indexer, virt_addr);

	struct PageTableEntry pte;
	pte = page_table->entries[indexer.page_directory_pointer_index];

	struct PageTable *pdp;

	if (!pte.present) {
		pdp = (void *)pmm_allocz();
		pte.address = (uintptr_t)pdp >> 12;
		pte.present = 1;
		pte.writable = 1;
		page_table->entries[indexer.page_directory_pointer_index] = pte;
	} else {
		pdp = pte.address << 12;
	}

	pte = pdp->entries[indexer.page_directory_index];
	struct PageTable *pd;

	if (!pte.present) {
		pd = (void *)pmm_allocz();
		pte.address = pd >> 12;
		pte.present = 1;
		pte.writable = 1;
		pdp->entries[indexer.page_directory_index] = pte;
	} else {
		pd = pte.address << 12;
	}

	pte = pd->entries[indexer.page_table_index];
	struct PageTable *pt;

	if (!pte.present) {
		pt = (void *)pmm_allocz();
		pte.address = pt >> 12;
		pte.present = 1;
		pte.writable = 1;
		pd->entries[indexer.page_directory_index] = pte;
	} else {
		pt = pte.address << 12;
	}

	pte = pt->entries[indexer.page_index];
	pte.address = phys_addr >> 12;
	pte.present = 1;
	pte.writable = 1;
	pt->entries[indexer.page_index] = pte;

}

void vmm_unmap(struct PageTable *page_table, void *virt) 
{

}

struct PageTable *vmm_create_new_address_space() 
{
	return NULL;
}

void *cr3_read() 
{
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
