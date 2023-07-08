.globl _start
_start:
pop %ax
pop %dx
shl $16, %edx
pop %dx
pop %ax
shl $16, %eax
pop %ax
