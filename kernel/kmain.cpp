#include "monitor.h"
#include "uint.h"
#include "video.h"
#include "sysinfo.h"
void memset(void * ptr, uint8_t val, uint32_t size) {
	for(uint32_t i = 0; i < size; i++) ((char *)ptr)[i] = val;
}
extern "C" void kmain()
{
	sysinfo();
	
	vbe_info_structure * tmp = (vbe_info_structure *)0x600;
	if(!get_vbe_info(tmp)) print_str("getting vbe info was unsuccesful", DEFAULT_COLOR);
	vbe_info_structure vbe_info = (*tmp);
	int num_modes = 0;
	uint16_t * video_modes = (uint16_t *)(((vbe_info.video_modes>>16)<<4) + vbe_info.video_modes&0xFFFF);
	while(video_modes[num_modes]!=0xFFFF) num_modes++;
	
	print_str("Number of video modes:", DEFAULT_COLOR);
	print_dec(num_modes, DEFAULT_COLOR);
	end_line();
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
	print_str("real mode bios successful!", DEFAULT_COLOR);
}
