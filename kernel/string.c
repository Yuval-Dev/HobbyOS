#include"string.h"
void memset(uint8_t * ptr, uint8_t val, uint32_t size) {
	for(uint32_t i = 0; i < size; i++) ptr[i] = val;
}
