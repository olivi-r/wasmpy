.globl _start
_start:
fldl (%rsp)
add $4, %rsp
fabs
fisttpl (%rsp)
