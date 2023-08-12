.globl _start
_start:
pop %eax
pop %ecx
add %eax, (%esp)
adc %ecx, 4(%esp)
