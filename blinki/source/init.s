
.section .init @defined in linker-file
.global _start 

_start:
  mov sp, #0x8000
	bl main @first part of main

.section .text @will hold code

tloop$:
	b tloop$
