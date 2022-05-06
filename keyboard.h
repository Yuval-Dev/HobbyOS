#ifndef KEYBOARD_H
#define KEYBOARD_H
#include"io.h"
char await_key() {
	char ret;
	while(1) {
		while(inb(0x64) & 1);
		ret = inb(0x60);
		if(ret) return ret;
		else continue;
	}
}
#endif
