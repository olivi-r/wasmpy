.globl _start
_start:
pop %rcx
rolq %cl, (%rsp)
