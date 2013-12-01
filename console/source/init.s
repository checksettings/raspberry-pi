@ Writes the 'font.bin' file into the kernel image
  .section .rodata
  .global font
  .align  4
font:
  .incbin "font.bin"
  .global fontsize
  .align  4
fontsize:
  .int    fontsize - font

@set address of kernel_main in raspbootin
.global _jump_to_raspbootin 
.balign 4
_jump_to_raspbootin :
  .word 0x02000068

.section .init @defined in linker-file
.global _start 

_start:
  mov sp, #0x8000
	bl main @first part of main
  
@.section .text @will hold code

tloop$:
	b tloop$
