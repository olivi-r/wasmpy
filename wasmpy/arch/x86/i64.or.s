.globl _start
_start:
pop %eax
pop %ecx
or %eax, (%esp)
or %ecx, 4(%esp)
