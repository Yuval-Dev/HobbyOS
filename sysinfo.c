#include"sysinfo.h"
#include"monitor.h"
#include"cpuinfo.h"
#include"video.h"
void sysinfo() {
	reset_cursor();
	print_str("System Information: ");
	end_line();
	cpuinfo cpu = cpuid(0);
	print_str("CPU Vendor: ");
	uint32_t data[3] = {cpu.ebx, cpu.edx, cpu.ecx};
	for(int i = 0; i < 3; i++) for(int j = 0; j < 4; j++) {
		put_char((data[i]>>(j*8))&0xFF, DEFAULT_COLOR);
	}
	end_line();
	print_str("Number of Video Modes: ");
	
	vbe_info_structure * tmp = (vbe_info_structure *)0x600;
	if(!get_vbe_info(tmp)) print_str("getting vbe info was unsuccesful", DEFAULT_COLOR);
	vbe_info_structure vbe_info = (*tmp);
	int num_modes = 0;
	uint16_t * video_modes = (uint16_t *)(((vbe_info.video_modes>>16)<<4) + vbe_info.video_modes&0xFFFF);
	vbe_mode_info_structure structs[10];
	while(video_modes[num_modes]!=0xFFFF) {
		if(num_modes<10) {
			get_vbe_mode_info((vbe_mode_info_structure *)0x1600, video_modes[num_modes]);
			structs[num_modes] = ((vbe_mode_info_structure *)0x1600)[0];
		}	
		num_modes++;
	}
	
	print_dec(num_modes);
	if(num_modes<=10) print_str(", printing all of them...");
	else print_str(", printing the first 10...");
	end_line();
	for(int i = 0; i < 10 && i < num_modes; i++) {
		dump_mode_info(&structs[i]);
		end_line();
	}
}


