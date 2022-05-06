#include"bios_int.h"
regs16_t bios_int(uint8_t code, regs16_t regs) {
	pmode_bios_interrupt_registers = regs;
	pmode_bios_interrupt_code = code;
	pmode_bios_interrupt();
	return pmode_bios_interrupt_registers;
}
void bios_int(uint8_t code, regs16_t * regs) {
	(*regs) = bios_int(code, (*regs));
}
void bios_int(uint8_t code, regs16_t in, regs16_t * out) {
	(*out) = bios_int(code, in);
}
