.globl _start
_start:
popq %rax
andq %rax, (%rsp)
