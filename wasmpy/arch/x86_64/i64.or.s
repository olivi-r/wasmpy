.globl _start
_start:
popq %rax
orq %rax, (%rsp)
