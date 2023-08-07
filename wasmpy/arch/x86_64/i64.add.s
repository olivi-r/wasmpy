.globl _start
_start:
popq %rax
popq %rcx
addq %rcx, %rax
pushq %rax
