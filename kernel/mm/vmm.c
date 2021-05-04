#include <cernel/mm/vmm.h>
#include <cernel/mm/pmm.h>
#include <cernel/lib/print.h>
#include <stddef.h>
#include <stdint.h>

struct PageTable *pt_kernel;

size_t pages_allocated = 0;
size_t page_maped = 0;

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

void vmm_init(struct stivale_mmap_entry *mmap, uint64_t mmap_count, struct stivale_struct *stivale_struct)
{

        pt_kernel = (struct PageTable *)pmm_allocz();

		kprintf("%x", pt_kernel);

		kprintf("\nBefore the first for loop");

		for (size_t i = 0; i < get_memory_size(mmap, mmap_count); i += PAGE_SIZE) {
			vmm_map(pt_kernel, i, i);
		}

		kprintf("\nBetween first and second for loop");

		for (size_t i = 0; i <= 0x100000000; i += PAGE_SIZE) {
            vmm_map(pt_kernel, i + HIGHER_HALF, i);
        }

		kprintf("\nBetween second and third for loop\n");

        for (size_t i = 0; i <= 0x80000000; i += PAGE_SIZE) {
            vmm_map(pt_kernel, i + KERNEL_PHYS_OFFSET, i);
        }

		kprintf("After third for loop\n");

		kprintf("%x\n", pt_kernel);

        cr3_set((uintptr_t)pt_kernel);

		kprintf("%x\n", pt_kernel);
}

void vmm_map(struct PageTable *page_table, uintptr_t virt_addr, uintptr_t phys_addr) 
{
	struct AddressIndexer indexer;
	create_address_indexer(&indexer, virt_addr);

	dbg_printf("\n%u --- virt_addr: %u\n", page_maped, virt_addr);
	dbg_printf("pdp_i: %u / pd_i: %u / pt_u: %u / p_i: %u", indexer.page_directory_pointer_index, 
														indexer.page_directory_index,
														indexer.page_table_index,
														indexer.page_index);

	
	struct PageTableEntry pte;
	pte = page_table->entries[indexer.page_directory_pointer_index];

	struct PageTable *pdp;

	if (!pte.present) {
		pdp = (struct PageTable *)pmm_allocz();
		pages_allocated += 1;
		pte.addr = (uintptr_t)pdp >> 12;
		pte.present = 1;
		pte.writable = 1;
		page_table->entries[indexer.page_directory_pointer_index] = pte;
	} else {
		pdp = (struct PageTable *)((uintptr_t)pte.addr << 12);
	}

	pte = pdp->entries[indexer.page_directory_index];
	struct PageTable *pd;

	if (!pte.present) {
		pd = (struct PageTable *)pmm_allocz();
		pages_allocated += 1;
		pte.addr = (uintptr_t)pd >> 12;
		pte.present = 1;
		pte.writable = 1;
		pdp->entries[indexer.page_directory_index] = pte;
	} else {
		pd = (struct PageTable *)((uintptr_t)pte.addr << 12);
	}

	pte = pd->entries[indexer.page_table_index];
	struct PageTable *pt;

	if (!pte.present) {
		pt = (struct PageTable *)pmm_allocz();
		pages_allocated += 1;
		pte.addr = (uintptr_t)pt >> 12;
		pte.present = 1;
		pte.writable = 1;
		pd->entries[indexer.page_directory_index] = pte;
	} else {
		pt = (struct PageTable *)((uintptr_t)pte.addr << 12);
	}

	pte = pt->entries[indexer.page_index];
	pte.addr = (uint64_t)phys_addr;
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
	asm ("mov %0, %%cr3" : : "r" (value));
}

void tlb_flush(unsigned long addr) {
	asm volatile("invlpg (%0)" ::"r" (addr) : "memory");
}
