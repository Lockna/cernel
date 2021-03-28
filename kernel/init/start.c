#include <stdint.h>
#include <stddef.h>
#include <stivale/stivale.h>
#include <cernel/drivers/fb/framebuffer.h>
#include <debug/debug.h>
#include <cernel/init/gdt.h>
#include <cernel/init/idt.h>
#include <cernel/util/print.h>
#include <cernel/mm/pmm.h>

// We need to tell the stivale bootloader where we want our stack to be.
// We are going to allocate our stack as an uninitialised array in .bss.
static uint8_t stack[4096];

// The stivale specification says we need to define a "header structure".
// This structure needs to reside in the .stivalehdr ELF section in order
// for the bootloader to find it. We use this __attribute__ directive to
// tell the compiler to put the following structure in said section.
__attribute__((section(".stivalehdr"), used))
struct stivale_header stivale_hdr = {
    // Let's tell the bootloader where our stack is.
    // We need to add the sizeof(stack) since in x86(_64) the stack grows
    // downwards.
    .stack = (uintptr_t)stack + sizeof(stack),
    // These flags can be used to tell the bootloader to enable or disable
    // certain features. We turn on bit 0 as we are going to ask for a
    // graphical framebuffer. Not setting this bit will make the
    // bootloader default to a CGA-compatible text mode instead.
    .flags = (1 << 0),
    // We set all the framebuffer specifics to 0 as we want the bootloader
    // to pick the best it can.
    .framebuffer_width  = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp    = 0,
    // The entry_point member is used to specify an alternative entry
    // point that the bootloader should jump to instead of the executable's
    // ELF entry point. We do not care about that so we leave it zeroed.
    .entry_point = 0
};

void printStivale(struct stivale_struct *stivale)
{
	kprintf("stivale struct: \n");
	kprintf("cmdline: %s\n", stivale->cmdline);
	kprintf("framebuffer address: %p\n", stivale->framebuffer_addr);
	kprintf("framebuffer pitch: %u\n", stivale->framebuffer_pitch);
	kprintf("framebuffer width: %u\n", stivale->framebuffer_width);
	kprintf("framebuffer height: %u\n", stivale->framebuffer_height);
	kprintf("framebuffer bpp: %u\n", stivale->framebuffer_bpp);
	kprintf("rsdp: %p\n", stivale->rsdp);
	kprintf("epoch: %u\n", stivale->epoch);
	kprintf("flags: %u\n", stivale->flags);
}

struct mmap_entry {
    uint64_t base;      // Physical address of base of the memory section
    uint64_t length;    // Length of the section
    uint32_t type;      // Type (described below)
    uint32_t unused;
} __attribute__((packed));

void printMemoryMap(struct mmap_entry *mmap, int count)
{
	kprintf("----- Memory Map -----\n");

	for (int i = 0; i < count; i++) {
		struct mmap_entry current = mmap[i];

		kprintf("%p-%p length=%x type=", current.base, current.base + current.length, current.length);

		int type = mmap[i].type;

		if (type == 1) {
			kprintf("\"Usable RAM\"");
		} else if (type == 2) {
			kprintf("\"Reserved\"");
		} else if (type == 3) {
			kprintf("\"ACPI reclaimable\"");
		} else if (type == 4) {
			kprintf("\"ACIP NVS\"");
		} else if (type == 5) {
			kprintf("\"Bad memory\"");
		} else if (type == 10) {
			kprintf("\"Kernel/Modules\"");
		} else if (type == 0x1000) {
			kprintf("\"Bootloader Reclaimable\"");
		}
		
		kprintf("\n");
	}

	kprintf("----------------------\n");
}

// The following will be our kernel's entry point.
void _start(struct stivale_struct *stivale_struct) 
{
    // Let's get the address of the framebuffer.
    uint8_t *fb_addr = (uint32_t *)stivale_struct->framebuffer_addr;

    load_fb_driver(fb_addr, 
                stivale_struct->framebuffer_width, 
                stivale_struct->framebuffer_height, 
                stivale_struct->framebuffer_bpp,
                stivale_struct->framebuffer_pitch);
	
	puts("Initializing GDT...");
	gdt_init();
    puts("Done\n");
	puts("Loading GDT...");
	gdt_load();	
	puts("Done\n");
    idt_init();
	
	//printStivale(stivale_struct);
	//printMemoryMap(stivale_struct->memory_map_addr, stivale_struct->memory_map_entries);
	
	kprintf("Adress of _start: %p\n", _start);
	kprintf("Addres of stivale_struct: %p\n", stivale_struct);

	pmm_init((struct stivale_mmap_entry *)stivale_struct->memory_map_addr, 
			  							 stivale_struct->memory_map_entries);

    // We're done, just hang...
    while(1) {
        asm ("hlt");
    }
}
