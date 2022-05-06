CC=i686-elf-gcc
CFLAGS=-nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -fpermissive -I./include/ -T linker.ld
SOURCES=./kernel/*.cpp
AS=nasm
ASFLAGS=-f elf
NASM_SOURCES=$(wildcard ./asm/*.asm)
NASM_OBJECTS=$(wildcard ./asm/*.o)

./asm/%.o: ./asm/%.asm
	$(AS) $(ASFLAGS) $<

kernel: $(NASM_OBJECTS)
	$(CC) $(NASM_OBJECTS) $(SOURCES) $(CFLAGS) -o kernel.bin

run: kernel
	qemu-system-x86_64 -fda kernel.bin
