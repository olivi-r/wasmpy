.globl _start
_start:
pop %ecx
pop %eax
add %eax, 4(%esp)
adc %ecx, (%esp)
