#include"gdt.h"
#include"idt.h"
#include"bios_int.h"
#include"vbe.h"
extern void kmain()
{
	init_gdt();
	init_idt();
	init_vbe();
	init_kbd();
	print_str("0123456789<+_<+_<+_<+_<+_<+_<+_<+_<abcdefghijklmnopqrstuvwxyz");
	uint16_t * idk = 0xb8000;
	idk[0] = 0x0230;
	int k = 0;
	for(;;) {
		k++;
	}
	asm volatile("int $0x12");
}
