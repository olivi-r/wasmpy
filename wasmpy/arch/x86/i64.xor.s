.globl _start
_start:
popl %eax
popl %ecx
xor %eax, (%esp)
xor %ecx, 4(%esp)
