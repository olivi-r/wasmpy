.globl _start
_start:
pop %eax
pop %edx
cwde
cdq
push %edx
push %eax
