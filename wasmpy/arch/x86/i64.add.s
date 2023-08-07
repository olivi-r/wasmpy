.globl _start
_start:
popl %eax
popl %ecx
add 4(%esp), %ecx
adc (%esp), %eax
movl %ecx, 4(%esp)
movl %eax, (%esp)
