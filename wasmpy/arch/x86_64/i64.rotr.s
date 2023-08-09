.globl _start
_start:
pop %rcx
pop %rax
ror %cl, %rax
push %rax
