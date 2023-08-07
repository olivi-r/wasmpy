.globl _start
_start:
popl %eax
popl %ecx
add %ecx, %eax
pushl %eax
