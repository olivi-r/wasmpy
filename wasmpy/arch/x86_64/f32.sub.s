.globl _start
_start:
flds (%rsp)
add $4, %rsp
fsubrs (%rsp)
fstps (%rsp)
