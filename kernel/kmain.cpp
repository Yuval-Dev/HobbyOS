#include "monitor.h"
extern int kernel_load_address;
extern "C" void kmain()
{
	print_str("Stage 3 loaded! Hello world!", COLOR(COLOR_GREEN, COLOR_BLACK, NORMAL_TEXT));
	end_line();
	print_str("123456 in hex: ", DEFAULT_COLOR);
	print_hex(123456, DEFAULT_COLOR);
	end_line();
	bool is_prime[10000];
	int count = 0;
	for(int i = 0; i < 10000; i++) is_prime[i] = true;
	for(int i = 2; i < 10000; i++) {
		if(!is_prime[i]) continue;
		count++;
		if(count==100) {
			print_str("The 100th prime number is: ", DEFAULT_COLOR);
			print_dec(i, DEFAULT_COLOR);
		}
		for(int j = i*2; j < 10000; j+= i) is_prime[j] = false;
	}
}
