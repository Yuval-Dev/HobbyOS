#include "monitor.h"
extern int kernel_load_address;
extern "C" void kmain()
{
	print_str("hello world from cplusplus", COLOR(COLOR_GREEN, COLOR_BLACK, NORMAL_TEXT));
}
