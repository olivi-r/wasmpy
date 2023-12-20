.globl _start
_start:
pop %eax
pop %ecx
sub %eax, (%esp)
sbb %ecx, 4(%esp)
