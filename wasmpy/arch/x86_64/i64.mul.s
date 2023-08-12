.globl _start
_start:
pop %rax
imulq (%rsp)
push %rax
