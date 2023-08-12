.globl _start
_start:
pop %eax
pop %ecx
xor %eax, (%esp)
xor %ecx, 4(%esp)
