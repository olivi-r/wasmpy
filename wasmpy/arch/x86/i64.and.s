.globl _start
_start:
pop %eax
pop %ecx
and %eax, (%esp)
and %ecx, 4(%esp)
