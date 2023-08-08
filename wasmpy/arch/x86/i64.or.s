.globl _start
_start:
popl %eax
popl %ecx
orl %eax, (%esp)
orl %ecx, 4(%esp)
