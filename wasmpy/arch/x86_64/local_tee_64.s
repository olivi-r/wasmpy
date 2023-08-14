.globl _start
_start:
pop %rax
push %rax
mov %rax, 255(%rbp)
