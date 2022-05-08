CC=i686-elf-gcc
CFLAGS=-nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -fpermissive -I./include/ -T linker.ld
SOURCES=./kernel/*.c
AS=nasm
ASFLAGS=-f elf
NASM_SOURCES=$(wildcard ./asm/*.asm)
NASM_OBJECTS=$(NASM_SOURCES:.asm=.o)

./asm/%.o: ./asm/%.asm
	$(AS) $(ASFLAGS) $<

.PHONY: kernel
kernel: $(NASM_OBJECTS)
	$(CC) $(NASM_OBJECTS) $(SOURCES) $(CFLAGS) -o kernel.bin

.PHONY: run
run: kernel
	qemu-system-x86_64 -fda kernel.bin
