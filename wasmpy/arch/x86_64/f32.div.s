.globl _start
_start:
flds (%rsp)
add $4, %rsp
fdivrs (%rsp)
fsts (%rsp)
