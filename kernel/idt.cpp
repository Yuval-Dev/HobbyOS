#include"idt.h"
#include"keyboard.h"
#include"monitor.h"
#include"video.h"
__attribute__((aligned(0x10))) static idt_entry_t idt[256];
static idt_ptr_t idt_ptr;
extern void* isr_stub_table[];

extern "C" void exception_handler() {
}
void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];
 
    descriptor->isr_low = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}
void test_func() {
	for(int i = 0; i < 100; i++) for(int j = 0; j < 100; j++) draw_pixel(i, j, i, j, i+j);
}
void idt_init() {
    idt_ptr.base = (uint32_t *)&idt[0];
    idt_ptr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
 
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }
    idt_set_descriptor(0x40, test_func, 0x8E);
 
    __asm__ volatile ("lidt %0" : : "m"(idt_ptr));
    __asm__ volatile ("sti");
}
