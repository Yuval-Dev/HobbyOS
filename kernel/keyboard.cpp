#include"keyboard.h"
char await_key()
{
	regs16_t out = bios_int(0x16, (regs16_t){ax:0});
	return out.ax&0xFF;
}
