.globl _start
_start:
pop %ecx
pop %eax
sub %eax, 4(%esp)
sbb %ecx, (%esp)
