.globl _start
_start:
popl %eax
popl %ecx
xorl %eax, (%esp)
xorl %ecx, 4(%esp)
