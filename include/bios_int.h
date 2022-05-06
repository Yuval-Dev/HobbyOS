#ifndef BIOS_INT_H
#define BIOS_INT_H
#include"uint.h"
typedef struct __attribute__ ((packed)) {
	uint16_t di, si, bp, sp, bc, dx, cx, ax, gs, fs, es, ds, eflags;
} regs16_t;
extern regs16_t pmode_bios_interrupt_registers;
extern uint8_t pmode_bios_interrupt_code;
extern "C" void pmode_bios_interrupt();
regs16_t bios_int(uint8_t, regs16_t);
void bios_int(uint8_t, regs16_t *);
void bios_int(uint8_t, regs16_t, regs16_t *);
#endif
