.globl _start
_start:
flds (%rsp)
addq $4, %rsp
fadds (%rsp)
fstps (%rsp)
