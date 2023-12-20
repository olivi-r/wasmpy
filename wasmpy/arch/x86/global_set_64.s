.globl _start
_start:
pop %eax
mov %eax, (0xff000000)
pop %eax
mov %eax, (0xfe000000)
