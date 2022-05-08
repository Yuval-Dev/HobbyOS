#include"monitor.h"
#include"gdt.h"
#include"idt.h"
#include"bios_int.h"
#include"vbe.h"
extern void kmain()
{
	init_gdt();
	init_idt();
	init_kbd();
	init_vbe();
	asm volatile("int $0x10");
	int k = 0;
	for(;;) {
		k++;
	}
	asm volatile("int $0x12");
}
