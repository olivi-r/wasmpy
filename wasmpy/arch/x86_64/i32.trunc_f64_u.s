.globl _start
_start:
fldl (%rsp)
addq $4, %rsp
fabs
fisttpl (%rsp)
