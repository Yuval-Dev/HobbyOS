#include"monitor.h"
#include"gdt.h"
extern void kmain()
{
	init_gdt();
}
