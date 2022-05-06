OUTPUT_FORMAT("binary")
ENTRY(start)
MEMORY
{
  real_mode_ivt (R) : ORIGIN = 0x00000000, LENGTH = 1k
  bios_data_area (R) : ORIGIN = 0x00000400, LENGTH = 256
  free_memory_low (WX) : ORIGIN = 0x00000500, LENGTH = 0x7700
  boot_sector (WX) : ORIGIN = 0x00007c00, LENGTH = 0xFFFFFFFF
  free_memory_mid (WX) : ORIGIN = 0x00007e00, LENGTH = 480k + 512
  extended_bios_data_area (R) : ORIGIN = 0x00080000, LENGTH = 128k
  video_display_memory (W) : ORIGIN = 0x000A0000, LENGTH = 128k
  video_bios (W) : ORIGIN = 0x000C0000, LENGTH = 32k
  bios_expansions (R) : ORIGIN = 0x000C8000, LENGTH = 160k
  motherboard_bios (R) : ORIGIN = 0x000F0000, LENGTH = 64k
  high_memory (WX) : ORIGIN = 0x00100000, LENGTH = 14m
}

SECTIONS
{
  .boot :
  {
    *(.boot)
    . = ALIGN(512);
  } > boot_sector AT>boot_sector
  .boot2 :
  {
    *(.boot2)
    . = ALIGN(512);
  } > free_memory_mid AT>boot_sector
  .text :
  {
    code = .; _code = .; __code = .;
    *(.text)
    . = ALIGN(512);
  } > high_memory AT>boot_sector

  .data :
  {
     data = .; _data = .; __data = .;
     *(.data)
     *(.rodata)
    . = ALIGN(512);
  } > high_memory AT>boot_sector

  .bss :
  {
    bss = .; _bss = .; __bss = .;:
    *(.bss)
    . = ALIGN(512);
  } > high_memory AT>boot_sector
  kernel_load_address = LOADADDR(.text);
  kernel_exec_address = ADDR(.text);
  kernel_exec_end = .;
  kernel_load_sector_count = (kernel_exec_end-kernel_exec_address)/512;
  real_mode_ivt = ORIGIN(real_mode_ivt);
  end = .; _end = .; __end = .;
}
/* NOTE:
 * The current boot sector supports up to 85KiB total OS size, and 84KiB of third stage code.
 * this is easily circumvented by simply increaseing the number of sectors read.
 */
