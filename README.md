# HobbyOS
 just another hobby OS
 
# Information about the files (ordered by function):

 boot.asm - the 512 byte sector that gets loaded by bios when the device turns on.
 
  It is responsible for two things, first: to allow transfer to protected mode, 
  
  second: to facilitate the return to real 16 bit mode whenever necessary (see bios_int.h).
  
 kmain.cpp - the function kmain contained within this file is called by boot.asm, it is the main file for the OS.
 
 monitor.cpp - this file utilizes the VGA text buffer to allow for printing characters and strings with colors
 
 bios_int.cpp - contains the bios_int function which allows us to access bios interrupts from protected mode
 
 video.cpp - contains code for detecting and setting video modes, as well as writing pixel data
 
 sysinfo.cpp - no functional purpose other than providing some information about the system
 
 cpuinfo.h - includes an abstraction from assembly to c of the cpuid instruction
 
 io.h includes an abstraction from assembly to c of the inb and outb instructions
 
 keyboard.h - has await_key() using bios interrupts
 
 uint.h - basic uintXX_t definitions
 
 Makefile: use "make kernel" to compile the code. use "make run" to compile and run the code.
 
# Notes:

 There is an issue with writing to the VGA buffer that frequently leads to crashes.
 
 The likely source of this issue is unhandled ISRs, and therefore the likely solution
 
 would be to write ISR handlers. This is a top priority to-do.
 
# Dependencies:

 qemu - (install from qemu.org and add to path)
 
 nasm - (isntall from nasm.us and add to path)
 
 i686-elf-gcc - (see osdev.org tutorial on compiling a cross compiler)
 
