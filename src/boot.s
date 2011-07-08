;
; boot.s -- kernel start location and definition of multiboot header
;

MBOOT_PAGE_ALIGN	equ 1<<0	; Load kernel and modules on a page boundary
MBOOT_MEM_INFO		equ 1<<1	; Provide kernel with memory info
MBOOT_HEADER_MAGIC	equ 0x1BADB002	; Multiboot Magic
MBOOT_HEADER_FLAGS	equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM		equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]				; All instructions should be 32 bit

[GLOBAL mboot]				; Make 'mboot' header accessible from C code
[EXTERN code]				; Start of '.text" section
[EXTERN bss]				; START of '.bss' section
[EXTERN end]				; End of last loadable section

mboot:
	dd	MBOOT_HEADER_MAGIC	; GRUB will search for this value on each
					; 4-byte boundary in your kernel file
	dd 	MBOOT_HEADER_FLAGS	; How GRUB should load your file / settings
	dd	MBOOT_CHECKSUM		; To ensure that the above values are correct

	dd	mboot			; Location of this descriptor
	dd	code			; Start of kernel '.text' (code) section
	dd	bss			; End of kernel '.data' section
	dd	end			; End of kernel
	dd	start			; Kernel entry point (initial EIP)

[GLOBAL start]				; Kernel entry point
[EXTERN main]				; This is the entry point of out C code

start:
	push	ebx			; Load multiboot header location

	; Execute kernel:
	cli				; Disable interrupts
	call main			; call out main() function
	jmp $				; Loop and random execution!
