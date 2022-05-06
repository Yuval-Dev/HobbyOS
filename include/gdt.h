#ifndef GDT_H
#define GDT_H
typedef struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_midd;
	uint8_t access;
	uint8_t granularity;
	uint8_t base_high;
} __attribute__ ((packed)) gdt_entry;
typedef struct gdt_pointer {
	uint16_t limit;
	uint16_t base;
} __attribute__ ((packed)) gdt_pointer;
#endif
