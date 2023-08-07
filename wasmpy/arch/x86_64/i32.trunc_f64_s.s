.globl _start
_start:
fldl (%rsp)
addq $4, %rsp
fisttpl (%rsp)
