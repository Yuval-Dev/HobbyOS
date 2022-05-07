#include "sysinfo.h"
#include "video.h"
#include "idt.h"
#include "keyboard.h"
#include "monitor.h"
extern "C" void kmain()
{
	init_video_driver();
	idt_init();
	//asm volatile("int $0x40");
}
