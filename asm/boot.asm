[SECTION .boot]
[GLOBAL start]
[BITS 16]
start:
	mov ax, 0x2401
	int 0x15 ;enables the A20 line through bios

	mov ax, 0x3
	int 0x10 ;I have no fucking clue what this does, just keep it bc it works


	mov [disk],dl ;saves the booted disk number passed by bios

	mov ah, 0x2             ;to read sectors
	mov al, 0xF               ;number of sectors to read
	mov ch, 0               ;cylinder number
	mov dh, 0               ;head number
	mov cl, 2               ;sector number
	mov dl, [disk]          ;disk number (the one we booted off of)
	mov bx, copy_target     ;where to copy the read sectors to
	int 0x13                ;create the interupt (0x13)
	cli                     ;clear interupts
	lgdt [gdt_pointer]      ;load the global descriptor table (go into pm)
	mov eax, cr0
	or eax,0x1              ;set the first bit of the cr0 reg
	mov cr0, eax
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp CODE_SEG:disk_loaded_stage
gdt_start:
	dq 0x0
gdt_code:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10011010b
	db 11001111b
	db 0x0
gdt_data:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10010010b
	db 11001111b
	db 0x0
gdt_end:
gdt_pointer:
	dw gdt_end - gdt_start
	dd gdt_start
disk:
	db 0x0
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

times 510 - ($-$$) db 0
dw 0xaa55
[SECTION .boot2]
[BITS 32]
[EXTERN kmain]
[EXTERN kernel_load_address]
[EXTERN kernel_exec_address]
copy_target:
  hello: db "Second Stage Loaded",0
  dq kernel_load_address
disk_loaded_stage:
  mov ebx, kernel_exec_address
  mov esi, kernel_load_address
  .loop2:
    lodsb
    cmp esi, 0x10000
    je after
    mov byte [ebx], al
    inc ebx
    jmp .loop2 
  after:
  mov esi,hello
  mov ebx,0xb8000
  .loop:
    lodsb
    or al,al
    jz halt
    or eax,0x0F00
    mov word [ebx], ax
    add ebx,2
    jmp .loop
halt:
  mov esp, 0x200000
  call kmain
  cli
  hlt
