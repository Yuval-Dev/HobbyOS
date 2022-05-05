#include "monitor.h"
extern "C" void kmain()
{
	print_str("hello world from cplusplus", COLOR(COLOR_GREEN, COLOR_BLACK, NORMAL_TEXT));
}
