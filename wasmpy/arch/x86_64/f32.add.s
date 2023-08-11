.globl _start
_start:
flds (%rsp)
add $4, %rsp
fadds (%rsp)
fstps (%rsp)
