#include"bios_int.h"
void bios_int_ptr(uint8_t code, regs16_t * regs) {
	(*pmode_bios_interrupt_registers) = (*regs);
	pmode_bios_interrupt();
	(*regs) = (*pmode_bios_interrupt_registers);
}
regs16_t bios_int_value(uint8_t code, regs16_t regs) {
	(*pmode_bios_interrupt_registers) = regs;
	pmode_bios_interrupt();
	return (*pmode_bios_interrupt_registers);
}
