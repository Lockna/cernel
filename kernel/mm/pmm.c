#include <stivale/stivale.h>
#include <cernel/mm/pmm.h>
#include <stdint.h>
#include <cernel/util/print.h>
#include <stddef.h>

#define USABLE_RAM 1

uint64_t bitmap_start;
uint64_t page_count;

struct stivale_mmap_entry *usable_ram_arr = NULL;
uint32_t usable_ram_entries;

static uint64_t page_number_to_address(uint64_t page_number)
{
	uint64_t ret = 0;

	for (uint32_t i = 0; i < usable_ram_entries; i++) {
		uint32_t current_page_count = (uint64_t)(usable_ram_arr[i].length) / PAGE_SIZE;
		
		if (page_number < current_page_count) {
			// calculate base address and return it
			ret = usable_ram_arr[i].base + page_number * PAGE_SIZE;
			break;
		} else {
			page_number -= current_page_count;
		}
	}

	return ret;
}

static uint64_t address_to_page_number(uint64_t base)
{
	uint64_t ret = 0;
	
	for (uint32_t i = 0; i < usable_ram_entries; i++) {
		if (base >= usable_ram_arr[i].base 
		 && base < usable_ram_arr[i].base + usable_ram_arr[i].length) {
			ret += (base - usable_ram_arr[i].base) / PAGE_SIZE;
			break;	
		} else {
			ret += usable_ram_arr[i].length / PAGE_SIZE;
		}
	}

	return ret;
}

void pmm_init(struct stivale_mmap_entry *memory_map_addr, uint32_t count)
{
	kprintf("MMAP adress: %p\n", memory_map_addr);
	kprintf("MMAP count: %u\n", count);

	usable_ram_entries = 0;
	usable_ram_arr = NULL;
	page_count = 0;

	// copy usable ram entries to an array
	// which is placed at the start of the first page
	// calculate the number of pages
	for (uint32_t i = 0; i < count; i++) {
		if (memory_map_addr[i].type == USABLE_RAM) {
			if (usable_ram_arr == NULL)
				usable_ram_arr = (struct stivale_mmap_entry *)memory_map_addr[i].base;

			usable_ram_arr[usable_ram_entries] = memory_map_addr[i];
			usable_ram_entries++;

			page_count += memory_map_addr[i].length / PAGE_SIZE;
		}
	}

	kprintf("Usable MMAP adress: %p\n", usable_ram_arr);
	kprintf("Usable MMAP count:  %u\n", usable_ram_entries);

	// bitmap starts at the beginning of the 2nd page
	bitmap_start = (uint64_t)(usable_ram_arr) + PAGE_SIZE;
	
	kprintf("Page size: %d\n", PAGE_SIZE);
	kprintf("Bitmap address: %p\n", bitmap_start);
	kprintf("Page count: %u\n", page_count);

	for (uint32_t i = 0; i < page_count; i++) {
		if (i != address_to_page_number(page_number_to_address(i))) {
			kprintf("fuck: %u\n", i);
			break;
		}
	}

	kprintf("It worked!!!");
}

uint64_t pmm_allocate_page()
{

}

int pmm_free_page(uint64_t page_addr)
{

}
