#pragma once

#include <stdint.h>

// the struct for the GDT
// this structure will later be loaded via lgdt
struct gdt_ptr {
	uint16_t size;   // size of the table subtracted by 1
	uint64_t offset; // linear address to the table
}__attribute__((packed));

struct gdt_entry {
	uint16_t limit_lo;
	uint16_t base_lo;
	uint8_t  base_mid;
	uint8_t  access;
	uint8_t  limit_hi:4;
	uint8_t  flags:4;
	uint8_t  base_hi;
}__attribute__((packed));

struct tss_entry {
    uint16_t length;
    uint16_t base_low16;
    uint8_t  base_mid8;
    uint8_t  flags1;
    uint8_t  flags2;
    uint8_t  base_high8;
    uint32_t base_upper32;
    uint32_t reserved;
} __attribute__((packed));

struct GDT {
    struct gdt_entry entries[5];
    struct tss_entry tss;
} __attribute__((packed));

static struct GDT gdt;
static struct gdt_ptr gdt_pointer;

// initialize the GDT
void gdt_init();

// creates a `gdt_entry` from parameters
struct gdt_entry gdt_create_entry(uint32_t limit, uint32_t base, uint8_t access, uint8_t flags);

// load the GDT
void gdt_load();
