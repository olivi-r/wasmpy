.globl _start
_start:
flds (%rsp)
addq $4, %rsp
fmuls (%rsp)
fsts (%rsp)
