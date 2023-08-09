.globl _start
_start:
pop %rcx
pop %rax
rol %cl, %rax
push %rax
