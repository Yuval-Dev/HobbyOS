#ifndef CPUINFO_H
#define CPUINFO_H
#include"uint.h"
struct cpuinfo {
	uint32_t eax, ebx, ecx, edx;
};
static inline void cpuid(int code, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d) {
	asm volatile("cpuid":"=a"(*a),"=b"(*b),"=c"(*c),"=d"(*d):"a"(code):);
}
cpuinfo cpuid(int code) {
	cpuinfo tmp;
	cpuid(code, &tmp.eax, &tmp.ebx, &tmp.ecx, &tmp.edx);
	return tmp;
}
#endif
