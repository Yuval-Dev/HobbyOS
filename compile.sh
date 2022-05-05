nasm -f elf ./asm/*.asm
i686-elf-gcc ./kernel/*.cpp ./asm/*.o -o kernel.bin -nostdlib -ffreestanding -std=c++11 -mno-red-zone -fno-exceptions -nostdlib -fno-rtti -Wall -Wextra -T linker.ld -I./include/
qemu-system-x86_64 -fda kernel.bin
