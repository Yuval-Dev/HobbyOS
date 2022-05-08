#pragma once
#include"idt.h"
void init_kbd();
static void keyboard_irq_handler(registers_t);
