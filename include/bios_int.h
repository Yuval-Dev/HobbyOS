#ifndef BIOS_INT_H
#define BIOS_INT_H
#include <stdint.h>
#define bios_int(port, regs) _Generic((regs), \
	regs16_t : bios_int_value, \
	regs16_t *: bios_int_ptr)(port,regs)
typedef struct {
	uint16_t di, si, bp, sp, bx, dx, cx, ax, gs, fs, es, ds, eflags;
} __attribute__ ((packed)) regs16_t;
extern regs16_t * pmode_bios_interrupt_registers;
extern uint8_t pmode_bios_interrupt_code;
extern void pmode_bios_interrupt();
void bios_int_ptr(uint8_t, regs16_t *);
regs16_t bios_int_value(uint8_t, regs16_t);
#endif
