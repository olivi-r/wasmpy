.globl _start
_start:
popl %eax
popl %ecx
andl %eax, (%esp)
andl %ecx, 4(%esp)
