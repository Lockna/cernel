#include <cernel/init/gdt.h>

struct GDT gdt;
struct gdt_ptr gdt_pointer;

void gdt_init()
{
	gdt.entries[0] = gdt_create_entry(0, 0, 0, 0);
	gdt.entries[1] = gdt_create_entry(0, 0, 0x9a, 0b1010); // kernel code
	gdt.entries[2] = gdt_create_entry(0, 0, 0x92, 0b1000); // kernel data
	gdt.entries[3] = gdt_create_entry(0, 0, 0x9a, 0b1010); // user code
	gdt.entries[4] = gdt_create_entry(0, 0, 0x92, 0b1000); // user data
	
	gdt.tss.length       = 104;
    gdt.tss.base_low16   = 0;
    gdt.tss.base_mid8    = 0;
    gdt.tss.flags1       = 0b10001001;
    gdt.tss.flags2       = 0;
    gdt.tss.base_high8   = 0;
    gdt.tss.base_upper32 = 0;
    gdt.tss.reserved     = 0;

	gdt_pointer.size = sizeof(gdt) - 1;
	gdt_pointer.offset = (uint64_t)&gdt;
}

struct gdt_entry gdt_create_entry(uint32_t limit, uint32_t base, uint8_t access, uint8_t flags)
{
	struct gdt_entry entry;
	
	entry.limit_lo = (uint16_t)limit;
	entry.limit_hi = (limit >> 16) & 0xf;

	entry.base_lo = (uint16_t)base;
	entry.base_mid = (base >> 16) & 0xff;
	entry.base_hi = (base >> 24) & 0xff;

	entry.access = access;
	entry.flags = flags & 0xf;

	return entry;
}

void gdt_load()
{
	asm volatile ("lgdt %0" : : "m" (gdt_pointer));

	asm("   movw $0x10, %ax    \n \
            movw %ax, %ds    \n \
            movw %ax, %es    \n \
            movw %ax, %fs    \n \
            movw %ax, %gs    \n \
			push $8 \n \
			push $next \n \
			retfq \n \
            next:        \n");
}