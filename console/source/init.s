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


.section .init @defined in linker-file
.global _start 

_start:
  mov sp, #0x8000
	bl main @first part of main

.section .text @will hold code

tloop$:
	b tloop$
