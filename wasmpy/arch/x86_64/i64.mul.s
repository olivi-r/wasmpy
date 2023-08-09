.globl _start
_start:
pop %rax
pop %rcx
imul %rcx, %rax
push %rax
