[SECTION .boot]
[GLOBAL start]
[GLOBAL pmode_bios_interrupt]
[GLOBAL pmode_bios_interrupt_registers]
[GLOBAL pmode_bios_interrupt_code]
[EXTERN kmain]
[EXTERN kernel_load_address]
[EXTERN kernel_exec_address]
[EXTERN kernel_exec_end]
[BITS 16]
start:
	xor ax, ax
	mov ds, ax
	mov es, ax
	jmp 0x0:adjust ;ensure that we are at 0x0000:0x7c00 as opposed to 0x07c0:0x0000 or something like that
adjust:
	mov ax, 0x2401
	int 0x15 ;enables the A20 line through bios. This lets us address up to 1MiB of memory

	mov ax, 0x3
	int 0x10 ;reset cursor position


	mov [disk],dl ;saves the booted disk number passed by bios


	mov ah, 0x2 ;to read sectors
	mov al, 0x41 ;number of sectors to read
	mov ch, 0 ;cylinder number
	mov dh, 0 ;head number
	mov cl, 2 ;sector number
	mov dl, [disk] ;disk number (the one we booted off of)
	mov bx, 0x7e00 ;where to copy the read sectors to
	int 0x13 ;create the interupt (0x13)
	
	mov ah, 0x2
	mov al, 0x80
	mov ch, 1
	mov cl, 5
	mov bx, 0x1000
	mov es, bx
	xor bx, bx
	int 0x13

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
	jmp CODE_SEG:stage2
[BITS 32]
stage2:
	mov ebx, kernel_exec_address
	mov esi, kernel_load_address
	.loop2:
		lodsb
		mov byte [ebx], al
		inc ebx
		cmp ebx, kernel_exec_end
		je .after
		jmp .loop2 
	.after:
		mov esp, 0x200000
		call kmain
		cli
		hlt
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
gdt_code_16:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10011010b
	db 00001111b
	db 0x0
gdt_data_16:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10010010b
	db 00001111b
	db 0x0
gdt_end:
gdt_pointer:
	dw gdt_end - gdt_start
	dd gdt_start
idt_pointer:
	dw 0x03FF
	dd 0x00000000
gdt_ptr:
	dq 0x0
idt_ptr:
	dq 0x0
disk:
	db 0x0
stack_ptr:
	dq 0x0
tmp_stack:
	dq 0x0
pmode_bios_interrupt_registers equ 0x7c00 - 33
pm_bios_stack equ 0x7c00 - 38
pmode_bios_interrupt:
	cli
	pusha
	mov [stack_ptr], esp
	mov esp, [pm_bios_stack]
	sgdt [gdt_ptr]
	sidt [idt_ptr]
	lgdt [gdt_pointer]
	jmp CODE_SEG_16:.pmode_16
.pmode_16:
[BITS 16]
	mov ax, DATA_SEG_16
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov eax, cr0
	and al, 0xFE
	mov cr0, eax
	jmp 0x0000:.real_mode
.real_mode:
	xor ax, ax
	mov ds, ax
	mov ss, ax
	lidt [idt_pointer]
	mov bx, 0x0870
	call resetpic
	mov [tmp_stack], sp
	mov sp, pmode_bios_interrupt_registers
	popa
	pop gs
	pop fs
	pop es
	pop ds
	sti
	db 0xCD
pmode_bios_interrupt_code: db 0x00
	cli
	add sp, 2
	pushf
	push ds
	push es
	push fs
	push gs
	pusha
	mov sp, [tmp_stack]
	mov bx, 0x2028
	call resetpic
	mov eax, cr0
	inc eax
	mov cr0, eax
	jmp dword CODE_SEG:.pmode32
[BITS 32]
.pmode32:
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	lgdt [gdt_ptr]
	lidt [idt_ptr]
	mov esp, [stack_ptr]
	popa
	sti
	ret
;source for resetpic:
resetpic:                                      ; reset's 8259 master and slave pic vectors
	push ax                                ; expects bh = master vector, bl = slave vector
	mov  al, 0x11                          ; 0x11 = ICW1_INIT | ICW1_ICW4
	out  0x20, al                          ; send ICW1 to master pic
	out  0xA0, al                          ; send ICW1 to slave pic
	mov  al, bh                            ; get master pic vector param
	out  0x21, al                          ; send ICW2 aka vector to master pic
	mov  al, bl                            ; get slave pic vector param
	out  0xA1, al                          ; send ICW2 aka vector to slave pic
	mov  al, 0x04                          ; 0x04 = set slave to IRQ2
	out  0x21, al                          ; send ICW3 to master pic
	shr  al, 1                             ; 0x02 = tell slave its on IRQ2 of master
	out  0xA1, al                          ; send ICW3 to slave pic
	shr  al, 1                             ; 0x01 = ICW4_8086
	out  0x21, al                          ; send ICW4 to master pic
	out  0xA1, al                          ; send ICW4 to slave pic
	pop  ax                                ; restore ax from stack
	ret                                    ; return to caller
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
CODE_SEG_16 equ gdt_code_16 - gdt_start
DATA_SEG_16 equ gdt_data_16 - gdt_start
times 510 - ($-$$) db 0
dw 0xaa55
