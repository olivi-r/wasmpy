.globl _start
_start:
pop %eax
pop %edx
cbw
cwde
cdq
push %edx
push %eax
