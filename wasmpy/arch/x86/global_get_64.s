.globl _start
_start:
mov (0xff000000), %eax
push %eax
mov (0xfe000000), %eax
push %eax
