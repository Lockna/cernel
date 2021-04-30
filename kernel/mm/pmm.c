#include <stivale/stivale.h>
#include <cernel/mm/pmm.h>
#include <stdint.h>
#include <stddef.h>

#define USABLE_RAM 1

uint64_t bitmap_start;
uint64_t page_count;

struct stivale_mmap_entry *usable_ram_arr = NULL;
uint32_t usable_ram_entries;

static void *pageframe_number_to_address(uint64_t pageframe_number)
{
	void *ret = NULL;

	for (uint32_t i = 0; i < usable_ram_entries; i++) {
		uint32_t current_page_count = (uint64_t)(usable_ram_arr[i].length) / PAGE_SIZE;
		
		if (pageframe_number < current_page_count) {
			// calculate base address and return it
			ret = (void *)(usable_ram_arr[i].base + pageframe_number * PAGE_SIZE);
			break;
		} else {
			pageframe_number -= current_page_count;
		}
	}

	return ret;
}

static uint64_t address_to_page_number(void *base)
{
	uint64_t ret = 0;
	
	for (uint32_t i = 0; i < usable_ram_entries; i++) {
		if ((uint64_t)base >= usable_ram_arr[i].base 
		 && (uint64_t)base < usable_ram_arr[i].base + usable_ram_arr[i].length) {
			ret += ((uint64_t)base - usable_ram_arr[i].base) / PAGE_SIZE;
			break;	
		} else {
			ret += usable_ram_arr[i].length / PAGE_SIZE;
		}
	}

	return ret;
}

static void set_bit(uint64_t pageframe_number)
{
	uint64_t start_page = 1;

	if (pageframe_number == 0) {
		*(unsigned char*)bitmap_start |= 1;
	}

	while (pageframe_number != 0) {
		if (pageframe_number >= PAGE_SIZE * 8) {
			pageframe_number -= PAGE_SIZE * 8;
			start_page++;
		} else {
			// bit is current page
			uint64_t *current_page_addr = (uint64_t *)pageframe_number_to_address(start_page);
			uint64_t offset = pageframe_number / 64;
			current_page_addr += offset;
			pageframe_number = pageframe_number % 64;

			*current_page_addr |= 1 << pageframe_number;
			pageframe_number = 0;
		}
	}
}

static void clear_bit(uint64_t pageframe_number)
{
	uint64_t start_page = 1;

	if (pageframe_number == 0) {
		*(unsigned char*)bitmap_start &= ~1;
	}

	while (pageframe_number != 0) {
		if (pageframe_number >= PAGE_SIZE * 8) {
			pageframe_number -= PAGE_SIZE * 8;
			start_page++;
		} else {
			// bit is current page
			uint64_t *current_page_addr = (uint64_t *)pageframe_number_to_address(start_page);
			uint64_t offset = pageframe_number / 64;
			current_page_addr += offset;
			pageframe_number = pageframe_number % 64;

			*current_page_addr &= ~(1 << pageframe_number);
			pageframe_number = 0;
		}
	}
}

void pmm_init(struct stivale_mmap_entry *memory_map_addr, uint32_t count)
{
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

	// bitmap starts at the beginning of the 2nd page
	bitmap_start = (uint64_t)(usable_ram_arr) + PAGE_SIZE;

	// allocate bitmap
	uint64_t size_bitmap = page_count / 8;
	uint64_t current_page = 1;	

	for (uint32_t i = 0; i < size_bitmap / PAGE_SIZE; i++) {
		// zero out page
		uint64_t *base = (uint64_t *)pageframe_number_to_address(current_page);

		for (int i = 0; i < PAGE_SIZE / 8; i++) {
			base[i] = 0;
		}

		current_page++;	
	}

	if (size_bitmap % PAGE_SIZE != 0) {
		// zero out page
		uint64_t *base = (uint64_t *)pageframe_number_to_address(current_page);
		
		for (int i = 0; i < PAGE_SIZE / 8; i++) {
            base[i] = 0;
        }		

		current_page++;
	}

	// mark pages 0 - current_page as used
	for (uint32_t i = 0; i < current_page; i++) {
		set_bit(i);
	}
}

void *pmm_alloc()
{
	void *ret = NULL;
	uint64_t current_page = 1; // bitmap is on first page

	while (1) {
		uint64_t *current_base = (uint64_t *)pageframe_number_to_address(current_page);

		for (uint32_t i = 0; i < PAGE_SIZE / 8; i++) {
			// check if end of bitmap is reached
			if ((current_page - 1) * PAGE_SIZE * 8 + i * 64 >= page_count)
				return 0;

			if (current_base[i] != 0xFFFFFFFFFFFFFFFF) {
				uint64_t current_page_index = (current_page - 1) * PAGE_SIZE * 8;
				current_page_index += i * 64;

				// find the bit which is zero
				// update `current_page_index`
				// set the bit to one

				for (uint32_t k = 0; k < 64; k++) {
					uint64_t temp = current_base[i];
					if ((temp >> k) % 2 == 0) {
						current_page_index += k;
						current_base[i] |= 1 << k;
						break;
					}
				}	

				ret = pageframe_number_to_address(current_page_index);

				if (current_page_index < page_count) {
					return ret;
				} else {
					return 0;
				}				
			}
		}

		current_page++;
	}		

	return ret;
}

// the page returned by pmm_alloc() must be identity mapped
void *pmm_allocz()
{
    void *ret = pmm_alloc();

    if (ret != NULL) {
		for (uint32_t i = 0; i < PAGE_SIZE / 8; i++) {
			*(uint64_t *)(ret + i) = 0;
		}
	}

    return ret;    
}

void pmm_free(void *page_addr)
{
	uint64_t page_number = address_to_page_number(page_addr);
	clear_bit(page_number);
}
