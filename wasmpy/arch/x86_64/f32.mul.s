.globl _start
_start:
flds (%rsp)
add $4, %rsp
fmuls (%rsp)
fsts (%rsp)
